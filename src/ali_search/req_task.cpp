#include "ali_search/req_task.h"
#include "base/basedefines.h"
#include "json/json.h"
#include "base/common.h"

namespace aos{

  const char DEFAULT_FAIL_ERROR[] = "{\"status\":\"FAIL\","
    "\"errors\":[{\"code\":0,\"message\":\"Post data build faild\"}],"
    "\"request_id\":\"0\"}";

  BaseReqTask::BaseReqTask(AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &api_url,
    HttpMethod http_method)
    :ppmp_(ppmp), ag_context_(ag_context),
    http_method_(http_method),
    api_url_(api_url),
    res_value_(new ResValue()){
    ppmp_->CopyPublicPart(keyvalues_);
  }

  BaseReqTask::~BaseReqTask(){

  }

  bool BaseReqTask::AddKeyValue(
    const std::string &key, const std::string &value){
    KVSInsertResult res = keyvalues_.insert(std::make_pair(key, value));
    if (res.second == false){
      LOG_ERROR << "Add key value error, maybe the same key aready exsits!";
    }
    return res.second;
  }

  void BaseReqTask::BuildReqUrl(std::string &result){
    result.append(ppmp_->api_domain());
    AddApiUrl(result);
    result.push_back('?');
  }

  void BaseReqTask::BuildRequestParames(std::string &parames){
    GeneratorCCQS(parames);
    std::string signature = GeneratorSignature(parames);
    parames += "&";
    parames += REQ_SIGNATURE;
    parames += "=";
    parames += HelpMethos::URLEncode(signature);
  }

  ResValue::Ptr BaseReqTask::SyncStart(){
    std::string req_url;
    std::string req_parames;
    // 1. 
    AddPrivateKeyValues();
    // 2. BuildReqUrl
    BuildReqUrl(req_url);
    // 3. BuildReqParames
    BuildRequestParames(req_parames);
    req_url.append(req_parames);

    DLOG_INFO << req_url;

    if (http_method_ == HTTP_GET){
      ag_context_->SyncProcessGetRequest(req_url, res_value_->rep_buffer());
    }
    else{
      std::string post_data;
      if (BuildPostData(post_data)){
        ag_context_->SyncProcessPostRequest(
          req_url, post_data, res_value_->rep_buffer());
      }
    }

    res_value_->ParseResult();
    return res_value_;
  }

  const std::string BaseReqTask::GeneratorSignature(const std::string &cqs){
    std::string signature;
    std::string sign_str;
    sign_str.append(HelpMethos::HttpMethodToStr(http_method_));
    sign_str.push_back('&');
    sign_str.append("%2F");
    sign_str.push_back('&');
    sign_str.append(HelpMethos::URLEncode(cqs));
    HelpMethos::HmacSha1ToBase64(ppmp_->access_key_secret(),
      sign_str, signature);
    return signature;
  }

  void BaseReqTask::GeneratorCCQS(std::string &ccqs){
    KeyValues::iterator iter = keyvalues_.begin();
    while (iter != keyvalues_.end()){
      ccqs.append(iter->first);
      ccqs.push_back('=');
      ccqs.append(HelpMethos::URLEncode(iter->second));
      ++iter;
      if (iter != keyvalues_.end()){
        ccqs.push_back('&');
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  CreateAppTask::CreateAppTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name,
    const std::string &template_name)
    :BaseReqTask(ag_context, ppmp, APP_MANAGER_URL, HttpMethod::HTTP_GET),
    app_name_(app_name),
    template_name_(template_name){
  }

  CreateAppTask::~CreateAppTask(){

  }

  void CreateAppTask::AddApiUrl(std::string &result){
    result.append(api_url_);
    result.push_back('/');
    result.append(app_name_);
  }

  void CreateAppTask::AddPrivateKeyValues(){
    AddKeyValue(REQ_ACTION, APP_MANAGER_ACTION_CREATE);
    AddKeyValue(REQ_TEMPLATE, template_name_);
  }

  //////////////////////////////////////////////////////////////////////////////

  GetAppStatusTask::GetAppStatusTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name)
    :BaseReqTask(ag_context, ppmp, APP_MANAGER_URL, HttpMethod::HTTP_GET),
    app_name_(app_name){
  }

  GetAppStatusTask::~GetAppStatusTask(){

  }

  void GetAppStatusTask::AddApiUrl(std::string &result){
    result.append(api_url_);
    result.push_back('/');
    result.append(app_name_);
  }

  void GetAppStatusTask::AddPrivateKeyValues(){
    AddKeyValue(REQ_ACTION, APP_MANAGER_ACTION_STATUS);
  }

  //////////////////////////////////////////////////////////////////////////////
  DeleteAppTask::DeleteAppTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name)
    :BaseReqTask(ag_context, ppmp, APP_MANAGER_URL, HttpMethod::HTTP_GET),
    app_name_(app_name){
  }

  DeleteAppTask::~DeleteAppTask(){

  }

  void DeleteAppTask::AddApiUrl(std::string &result){
    result.append(api_url_);
    result.push_back('/');
    result.append(app_name_);
  }

  void DeleteAppTask::AddPrivateKeyValues(){
    AddKeyValue(REQ_ACTION, APP_MANAGER_ACTION_DELETE);
  }

  //////////////////////////////////////////////////////////////////////////////

  // ---------------------------------------------------------------------------
  PushItem::PushItem(PushItemType type, const std::string &id) :id_(id){
    item_json_[JSON_PUSH_CMD] = PushItemTypeToString(type);
    item_json_[JSON_PUSH_TIMESTAMP] = HelpMethos::GetUnixTimeStamp();
    fields_json_[JSON_PUSH_ITEM_ID] = id;
  }

  bool PushItem::AddField(const std::string &key, const std::string &value){
    try{
      fields_json_[key] = value;
    }
    catch (std::exception &e){
      LOG_ERROR << e.what();
      return false;
    }
    return true;
  }

  bool PushItem::AddField(const std::string &key, Json::Value &value){
    try{
      fields_json_[key] = value;
    }
    catch (std::exception &e){
      LOG_ERROR << e.what();
      return false;
    }
    return true;
  }
  Json::Value &PushItem::ToJson(){
    item_json_[JSON_PUSH_FIELDS] = fields_json_;
    return item_json_;
  }

  const std::string PushItem::PushItemTypeToString(PushItemType type){
    switch (type)
    {
    case aos::ITEM_TYPE_ADD:
      return "add";
      break;
    case aos::ITEM_TYPE_UPDATE:
      return "update";
      break;
    case aos::ITEM_TYPE_DELETE:
      return "delete";
      break;
    default:
      break;
    }
    ASSERT(false);
    return "update";
  }
  // ---------------------------------------------------------------------------

  PushIndexDocTask::PushIndexDocTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name,
    const std::string &table_name)
    :BaseReqTask(ag_context, ppmp, APP_PUSH_URL, HttpMethod::HTTP_POST),
    app_name_(app_name),
    table_name_(table_name){
  }

  PushIndexDocTask::~PushIndexDocTask(){

  }

  PushItem::Ptr PushIndexDocTask::AddItem(
    PushItemType type, const std::string &id){
    PushItem::Ptr item(new PushItem(type, id));
    items_.push_back(item);
    return item;
  }

  bool PushIndexDocTask::BuildPostData(std::string &post_data){
    if (items_.size() == 0){
      LOG_ERROR << "The push data is null";
      return false;
    }
    Json::Value items_json(Json::arrayValue);
    for (std::size_t i = 0; i < items_.size(); i++){
      items_json.append(items_[i]->ToJson());
    }
    Json::FastWriter fw;
    post_data.append(JSON_PUSH_ITEMS);
    post_data.push_back('=');
    post_data.append(fw.write(items_json));
    post_data = HelpMethos::URLEncode(post_data);
    LOG_INFO << post_data;
    return true;
  }

  void PushIndexDocTask::AddApiUrl(std::string &result){
    result.append(api_url_);
    result.push_back('/');
    result.append(app_name_);
  }

  void PushIndexDocTask::AddPrivateKeyValues(){
    AddKeyValue(REQ_ACTION, RES_ACTION_PUSH);
    AddKeyValue(RES_TABLE_NAME, table_name_);
  }

  //////////////////////////////////////////////////////////////////////////////
  SuggestTask::SuggestTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name,
    const std::string &query,
    const std::string &suggest_name,
    int hit_count)
    :BaseReqTask(ag_context, ppmp, APP_SUGGEST_URL, HttpMethod::HTTP_GET),
    app_name_(app_name),
    query_(query),
    suggest_name_(suggest_name),
    hit_count_(hit_count){
  }

  SuggestTask::~SuggestTask(){

  }

  void SuggestTask::AddApiUrl(std::string &result){
    result.append(api_url_);
  }

  void SuggestTask::AddPrivateKeyValues(){
    AddKeyValue(RES_SUGGEST_QUERY, query_);
    AddKeyValue(RES_SUGGEST_INDEX_NAME, app_name_);
    AddKeyValue(RES_SUGGEST_NAME, suggest_name_);
    AddKeyValue(RES_SUGGEST_HIT, std::to_string(hit_count_));
  }


  //////////////////////////////////////////////////////////////////////////////
  ReindexTask::ReindexTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name,
    const std::string &operate,
    const std::string &table_name)
    :BaseReqTask(ag_context, ppmp, APP_SUGGEST_URL, HttpMethod::HTTP_GET),
    app_name_(app_name),
    operate_(operate),
    table_name_(table_name){
  }

  ReindexTask::~ReindexTask(){

  }

  void ReindexTask::AddApiUrl(std::string &result){
    result.append(api_url_);
    result.push_back('/');
    result.append(app_name_);
  }

  void ReindexTask::AddPrivateKeyValues(){
    AddKeyValue(REQ_ACTION, RES_REINDEX_ACTION_CREATATASK);
    if (!operate_.empty()){
      AddKeyValue(RES_REINDEX_OPERATE, operate_);
      AddKeyValue(RES_REINDEX_TABLE_NAME, table_name_);

    }
  }

  //////////////////////////////////////////////////////////////////////////////
  GetErrorLogTask::GetErrorLogTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name,
    int page, int page_size, SortMode sort_mode)
    :BaseReqTask(ag_context, ppmp, APP_SUGGEST_URL, HttpMethod::HTTP_GET),
    app_name_(app_name),
    page_(page),
    page_size_(page_size),
    sort_mode_(sort_mode){
  }

  GetErrorLogTask::~GetErrorLogTask(){

  }

  void GetErrorLogTask::AddApiUrl(std::string &result){
    result.append(api_url_);
    result.push_back('/');
    result.append(app_name_);
  }

  void GetErrorLogTask::AddPrivateKeyValues(){
    AddKeyValue(RES_GEL_PAGE, std::to_string(page_));
    AddKeyValue(RES_GEL_PAGE_SIZE, std::to_string(page_size_));
    AddKeyValue(RES_GEL_SORT_MODE, SortModeToString(sort_mode_));
  }

  const std::string GetErrorLogTask::SortModeToString(SortMode sort_mode){
    switch (sort_mode)
    {
    case aos::SORT_ASC:
      return RES_GEL_SORT_MODE_ASC;
      break;
    case aos::SORT_DESC:
      return RES_GEL_SORT_MODE_DESC;
      break;
    default:
      break;
    }
    ASSERT(false);
    return RES_GEL_SORT_MODE_ASC;
  }

  //////////////////////////////////////////////////////////////////////////////
  //----------------------------------------------------------------------------
  QueryStanza::QueryStanza(const std::string &key_word,
    const std::string &index_name){
    value_.append(index_name);
    value_.push_back(':');
    value_.push_back('\'');
    value_.append(key_word);
    value_.push_back('\'');
  }

  QueryStanza::QueryStanza(const std::string &value) :value_(value){
  }

  QueryStanza::Ptr QueryStanza::OrdinaryUnion(QueryUnionType type,
    QueryStanza::Ptr query_stanza){
    ASSERT(query_stanza.get() != NULL);
    std::string new_value;
    new_value.append(value_);
    new_value.push_back(' ');
    new_value.append(UnionTypeToString(type));
    new_value.push_back(' ');
    new_value.append(query_stanza->value());
    return QueryStanza::Ptr(new QueryStanza(new_value));
  }

  QueryStanza::Ptr QueryStanza::PriorityUnion(QueryUnionType type,
    QueryStanza::Ptr query_stanza){
    ASSERT(query_stanza.get() != NULL);
    std::string new_value;
    new_value.push_back('(');
    new_value.append(value_);
    new_value.push_back(')');
    new_value.push_back(' ');
    new_value.append(UnionTypeToString(type));
    new_value.push_back(' ');
    new_value.append(query_stanza->value());
    return QueryStanza::Ptr(new QueryStanza(new_value));
  }

  const std::string QueryStanza::UnionTypeToString(QueryUnionType type){
    switch (type)
    {
    case aos::UNION_AND:
      return UNION_TYPE_AND;
      break;
    case aos::UNION_OR:
      return UNION_TYPE_OR;
      break;
    case aos::UNION_ANDNOT:
      return UNION_TYPE_ANDNOT;
      break;
    case aos::UNION_RANK:
      return UNION_TYPE_RANK;
      break;
    default:
      break;
    }
    ASSERT(false);
    return UNION_TYPE_AND;
  }
  //----------------------------------------------------------------------------

  FilterStanza::FilterStanza(const std::string &filter_express)
    :filter_express_(filter_express){

  }

  FilterStanza::Ptr FilterStanza::OrdinaryUnion(FilterUnionType type,
    FilterStanza::Ptr filter_stanza){
    std::string new_filter_stanza;
    new_filter_stanza.append(filter_express_);
    new_filter_stanza.push_back(' ');
    new_filter_stanza.append(FilterUnionTypeToString(type));
    new_filter_stanza.push_back(' ');
    new_filter_stanza.append(filter_stanza->filter_express());
    return FilterStanza::Ptr(new FilterStanza(new_filter_stanza));
  }

  FilterStanza::Ptr FilterStanza::PriorityUnion(FilterUnionType type,
    FilterStanza::Ptr filter_stanza){
    std::string new_filter_stanza;
    new_filter_stanza.push_back('(');
    new_filter_stanza.append(filter_express_);
    new_filter_stanza.push_back(')');
    new_filter_stanza.push_back(' ');
    new_filter_stanza.append(FilterUnionTypeToString(type));
    new_filter_stanza.push_back(' ');
    new_filter_stanza.append(filter_stanza->filter_express());
    return FilterStanza::Ptr(new FilterStanza(new_filter_stanza));
  }

  const std::string FilterStanza::FilterUnionTypeToString(FilterUnionType type){
    switch (type)
    {
    case aos::FILTER_UNION_TYPE_AND:
      return UNION_TYPE_AND;
      break;
    case aos::FILTER_UNION_TYPE_OR:
      return UNION_TYPE_OR;
      break;
    default:
      break;
    }
    ASSERT(false);
    return UNION_TYPE_AND;
  }
  //----------------------------------------------------------------------------
  SortStanza::SortStanza(const std::string &sort_express) 
    :sort_express_(sort_express){
  }

  SortStanza::Ptr SortStanza::UnionFilter(SortStanza::Ptr sort_stanza){
    std::string new_sort_stanza;
    new_sort_stanza.append(sort_express_);
    new_sort_stanza.push_back(';');
    new_sort_stanza.append(sort_stanza->sort_express());
    return SortStanza::Ptr(new SortStanza(new_sort_stanza));
  }


  //----------------------------------------------------------------------------
  AggregateStanza::AggregateStanza(const std::string &group_key)
    :group_key_(group_key),
    max_group_(0),
    agg_sampler_threshold_(0),
    agg_sampler_step_(0){
    ASSERT(group_key.empty());
  }

  const std::string &AggregateStanza::aggregate_express(){
    std::string express;
    // 1. group key
    express.append(QUERY_AGGREGATE_GROUP_KEY);
    express.push_back(':');
    express.append(group_key_);
    // 2. range
    for (std::set<std::string>::iterator iter = ranges_.begin();
      iter != ranges_.end(); ++iter){
      express.push_back(',');
      express.append(QUERY_AGGREGATE_RANGE);
      express.push_back(':');
      express.append(*iter);
    }
    // 3. agg_func
    if (agg_funcs_.size() > 0){
      express.push_back(',');
      express.append(QUERY_AGGREGATE_FUN);
      express.push_back(':');
      std::set<std::string>::iterator iter = agg_funcs_.begin();
      while (iter != agg_funcs_.end()){
        express.append(*iter);
        ++iter;
        if (iter != agg_funcs_.end()){
          express.push_back('#');
        }
      }
    }
    // 4. max_group
    if (max_group_){
      express.push_back(',');
      express.append(QUERY_AGGREGATE_MAX_GROUP);
      express.push_back(':');
      express.append(std::to_string(max_group_));
    }
    // 5. agg_filter
    if (!agg_filter_.empty()){
      express.push_back(',');
      express.append(QUERY_AGGREGATE_FILTER);
      express.push_back(':');
      express.append(agg_filter_);
    }
    // 6. agg_sampler_threshold
    if (agg_sampler_threshold_){
      express.push_back(',');
      express.append(QUERY_AGGREGATE_AST);
      express.push_back(':');
      express.append(std::to_string(agg_sampler_threshold_));
    }
    // 7. agg_sampler_step
    if (agg_sampler_step_){
      express.push_back(',');
      express.append(QUERY_AGGREGATE_ASS);
      express.push_back(':');
      express.append(std::to_string(agg_sampler_step_));
    }
    return express;
  }

  void AggregateStanza::AddAggRange(uint32 min, uint32 max){
    std::string range_str;
    range_str.append(QUERY_AGGREGATE_RANGE);
    range_str.push_back(':');
    range_str.append(std::to_string(min));
    range_str.push_back('~');
    range_str.append(std::to_string(max));
    ranges_.insert(range_str);
  }

  void AggregateStanza::ClearRange(){
    ranges_.clear();
  }

  void AggregateStanza::AddAggFunc(const std::string &func_express){
    agg_funcs_.insert(func_express);
  }

  void AggregateStanza::ClearAggFunc(){
    agg_funcs_.clear();
  }

  //----------------------------------------------------------------------------
  SearchSummary::SearchSummary(const std::string &summary_field,
    const std::string &summary_element,
    const std::string &summary_ellipsis,
    int summary_snipped)
    :summary_field_(summary_field),
    summary_element_(summary_element),
    summary_ellipsis_(summary_ellipsis),
    summary_snipped_(summary_snipped){
  }

  bool SearchSummary::ToSummaryString(std::string &summary){
    // 1. add the filed
    if (!summary_field_.empty()){
      AddKeyValueToString(summary, RES_SUMMARY_FIELD, summary_field_);
    }
    else{
      return false;
    }
    // 2. add element
    if (!summary_element_.empty()){
      AddKeyValueToString(summary, RES_SUMMARY_ELEMENT, summary_element_);
    }
    // 3. add ellipsis
    if (!summary_ellipsis_.empty()){
      AddKeyValueToString(summary, RES_SUMMARY_ELLIPSIS, summary_ellipsis_);
    }
    // 4. add snipped
    if (summary_snipped_ >= 1){
      AddKeyValueToString(summary, RES_SUMMARY_SNIPPED, 
        std::to_string(summary_snipped_));
    }
    // 5. add len
    if (summary_len_ > 0){
      AddKeyValueToString(summary, RES_SUMMARY_LEN,
        std::to_string(summary_len_));
    }
    // 6. add summary prefix and summary postfix
    if (!summary_prefix_.empty() && !summary_postfix_.empty()){
      AddKeyValueToString(summary, RES_SUMMARY_PREFIX, summary_prefix_);
      AddKeyValueToString(summary, RES_SUMMARY_POSTFIX, summary_postfix_);
    }
    return true;
  }

  void SearchSummary::AddKeyValueToString(std::string &str,
    const std::string &key, const std::string &value){
    str.append(key);
    str.push_back(':');
    str.append(value);
  }

  bool SearchSummary::set_summary_filed(const std::string &summary_filed){
    if (summary_filed.empty()){
      return false;
    }
    summary_field_ = summary_filed;
    return true;
  }

  bool SearchSummary::set_summary_element(const std::string &summary_element){
    if (summary_element.empty()){
      return false;
    }
    summary_element_ = summary_element;
    // clear the summary_prefix and summary postfix
    summary_prefix_.clear();
    summary_postfix_.clear();

    return true;
  }

  bool SearchSummary::set_summary_ellipsis(const std::string &summary_ellipsis){
    if (summary_ellipsis.empty()){
      return false;
    }
    summary_ellipsis_ = summary_ellipsis;
    return true;
  }

  bool SearchSummary::set_summary_snipped(uint32 summary_snipped){
    if (summary_snipped < 1){
      return false;
    }
    summary_snipped_ = summary_snipped;
    return true;
  }

  bool SearchSummary::set_summary_len(uint32 summary_len){
    summary_len_ = summary_len;
    return true;
  }

  bool SearchSummary::set_summary_prefix_postfix(
    const std::string &summary_prefix, const std::string &summary_postfix){
    if (summary_prefix.empty() || summary_postfix_.empty()){
      return false;
    }
    // set the summary_element with null
    summary_element_.clear();
    // Set the summary fix value
    summary_prefix_ = summary_prefix;
    summary_postfix_ = summary_postfix;
    return true;
  }


  //----------------------------------------------------------------------------


  SearchForm::SearchForm() :enable_summary_(false){

  }

  void SearchForm::AddSearchAppName(const std::string &app_name){
    index_app_names_.insert(app_name);
  }

  void SearchForm::RemoveSearchAppName(const std::string &app_name){
    if (index_app_names_.size() == 0){
      return;
    }
    std::set<std::string>::iterator iter = index_app_names_.find(app_name);
    if (iter != index_app_names_.end()){
      index_app_names_.erase(iter);
    }
  }

  void SearchForm::AddFetchFields(const std::string &app_name){
    fetch_fields_.insert(app_name);
  }

  void SearchForm::RemoveFetchFields(const std::string &app_name){
    if (fetch_fields_.size() == 0){
      return;
    }
    std::set<std::string>::iterator iter = fetch_fields_.find(app_name);
    if (iter != fetch_fields_.end()){
      fetch_fields_.erase(iter);
    }
  }

  SearchSummary::Ptr SearchForm::CreateAndEnableSummary(
    const std::string &summary_field,
    const std::string &summary_element,
    const std::string &summary_ellipsis,
    int summary_snipped){
    summary_.reset(new SearchSummary(summary_field, 
      summary_element, 
      summary_ellipsis,
      summary_snipped));
    enable_summary_ = true;
    return summary_;
  }

  bool SearchForm::EnableSummary(){
    if (summary_){
      enable_summary_ = true;
    }
    else{
      enable_summary_ = false;
    }
    return enable_summary_;
  }

  void SearchForm::DisableSummary(){
    enable_summary_ = false;
  }

}