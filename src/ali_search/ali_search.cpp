#include "ali_search/ali_search.h"
#include "curl/curl.h"

namespace aos{

  AliOpenSearch::Ptr AliOpenSearch::CreateAliOpenSearch(
    AosGlobalContext::Ptr ag_context,
    const std::string &api_domain,
    const std::string &access_key_id,
    const std::string &access_key_secret){
    AliOpenSearch::Ptr aosp(new AliOpenSearch(ag_context, api_domain,
      access_key_id, access_key_secret));
    return aosp;
  }

  AliOpenSearch::AliOpenSearch(AosGlobalContext::Ptr ag_context,
    const std::string &api_domain,
    const std::string &access_key_id, 
    const std::string &access_key_secret)
    :ppm_(new PublicPartManager(api_domain, access_key_id, access_key_secret)),
    ag_context_(ag_context){

  }

  AliOpenSearch::~AliOpenSearch(){
    // Uinit the curl library
    curl_global_cleanup();
  }

  ResValue::Ptr AliOpenSearch::CreateNewApp(const std::string &app_name,
    const std::string &template_name){
    CreateAppTask::Ptr task(new CreateAppTask(
      ag_context_, ppm_, 
      app_name, 
      template_name));

    return task->SyncStart();
  }

  ResValue::Ptr AliOpenSearch::GetAppStastus(
    const std::string &app_name){
    GetAppStatusTask::Ptr task(new GetAppStatusTask(
      ag_context_, ppm_,
      app_name
      ));

    return task->SyncStart();
  }

  ResValue::Ptr AliOpenSearch::DeleteApp(const std::string &app_name){
    DeleteAppTask::Ptr task(new DeleteAppTask(
      ag_context_, ppm_,
      app_name
      ));

    return task->SyncStart();
  }

  PushIndexDocTask::Ptr AliOpenSearch::BuildPushIndexDocTask(
    const std::string &app_name,
    const std::string &table_name){
    return PushIndexDocTask::Ptr(new PushIndexDocTask(
      ag_context_, ppm_,
      app_name,table_name
      ));
  }

  ResValue::Ptr AliOpenSearch::SuggestHit(const std::string &app_name,
    const std::string &query, const std::string &suggest_name,
    int hit_count){
    if (hit_count < 0 || hit_count > 10){
      LOG_ERROR << "the hit must be [1, 10]";
      return ResValue::Ptr();
    }
    SuggestTask::Ptr task(new SuggestTask(
      ag_context_, ppm_,
      app_name, query, suggest_name, hit_count
      ));

    return task->SyncStart();
  }

  ResValue::Ptr AliOpenSearch::ReindexApp(const std::string &app_name,
    const std::string &operate,
    const std::string &table_name){
    if (!operate.empty() && table_name.empty()){
      LOG_ERROR << "reinex operate must with a table";
      return ResValue::Ptr();
    }
    ReindexTask::Ptr task(new ReindexTask(
      ag_context_, ppm_, app_name, operate, table_name));

    return task->SyncStart();
  }

  ResValue::Ptr AliOpenSearch::GetAppErrorLog(const std::string &app_name,
    int page, int page_size, SortMode sort_mode){
    GetErrorLogTask::Ptr task(new GetErrorLogTask(
      ag_context_, ppm_, app_name, page, page_size, sort_mode));
    return task->SyncStart();
  }

  QueryStanza::Ptr AliOpenSearch::CreateQueryStanza(
    const std::string &index_name, const std::string &key_word){
    return QueryStanza::Ptr(new QueryStanza(key_word, index_name));
  }

  FilterStanza::Ptr AliOpenSearch::CreateFilterStanza(
    const std::string &filter_express){
    return FilterStanza::Ptr(new FilterStanza(filter_express));
  }

  SortStanza::Ptr AliOpenSearch::CreateSortStanza(
    const std::string &sort_express){
    return SortStanza::Ptr(new SortStanza(sort_express));
  }

}