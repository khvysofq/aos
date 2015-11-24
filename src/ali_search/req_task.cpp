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

  PushIndexDocTask::PushIndexDocTask(
    AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    const std::string &app_name,
    const std::string &table_name,
    PushForm::Ptr push_form)
    :BaseReqTask(ag_context, ppmp, APP_PUSH_URL, HttpMethod::HTTP_POST),
    app_name_(app_name),
    table_name_(table_name),
    push_form_(push_form){
  }

  PushIndexDocTask::~PushIndexDocTask(){

  }

  bool PushIndexDocTask::BuildPostData(std::string &post_data){
    if (push_form_->IsValue()){
      post_data.append(push_form_->Express());
      return true;
    }
    return false;
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
    : BaseReqTask(ag_context, ppmp, APP_SEARCH_URL, HttpMethod::HTTP_GET),
    search_form_(search_form){

  }

  SearchTask::SearchTask(AosGlobalContext::Ptr ag_context,
    PublicPartManager::Ptr ppmp,
    SearchForm::Ptr search_form,
    Scroll::Ptr scroll) 
    : BaseReqTask(ag_context, ppmp, APP_SEARCH_URL, HttpMethod::HTTP_GET),
    search_form_(search_form),
    scroll_(scroll){

  }

  SearchTask::~SearchTask(){

  }

  ResValue::Ptr SearchTask::SyncStart(){
    BaseReqTask::SyncStart();
    if (scroll_){
      scroll_->HandleResultMessage(res_value_->rep_json()[JSON_RESULT]);
    }
    return res_value_;
  }

  void SearchTask::AddApiUrl(std::string &result){
    result.append(api_url_);
  }

  void SearchTask::AddPrivateKeyValues(){
    std::map<std::string, std::string> kvs;
    if (scroll_ && scroll_->UpdateKeyValue(kvs)){
      // Do nothing
    }
    else{
      search_form_->UpdateKeyValue(kvs);
    }
    for (std::map<std::string, std::string>::iterator iter = kvs.begin();
      iter != kvs.end(); ++iter){
      AddKeyValue(iter->first, iter->second);
    }
  }

  //////////////////////////////////////////////////////////////////////////////

}