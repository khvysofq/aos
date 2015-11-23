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

  // ---------------------------------------------------------------------------
  SearchTask::SearchTask(AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    SearchForm::Ptr search_form)
    :BaseReqTask(ag_context, ppmp, APP_SEARCH_URL, HttpMethod::HTTP_GET),
    search_form_(search_form){

  }

  //////////////////////////////////////////////////////////////////////////////

}