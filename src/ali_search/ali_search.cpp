#include "ali_search/ali_search.h"
#include "base/logging.h"
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

  ResValue::Ptr AliOpenSearch::PushIndexDoc(const std::string &app_name,
    const std::string &table_name, PushForm::Ptr push_form){
    PushIndexDocTask::Ptr task(new PushIndexDocTask(
      ag_context_, ppm_, app_name, table_name, push_form));
    return task->SyncStart();
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

  ResValue::Ptr AliOpenSearch::Search(SearchForm::Ptr search_form){
    SearchTask::Ptr task(new SearchTask(ag_context_, ppm_, search_form));
    return task->SyncStart();
  }

  ResValue::Ptr AliOpenSearch::ScrollSearch(
    SearchForm::Ptr search_form, Scroll::Ptr scroll){
    SearchTask::Ptr task(new SearchTask(
      ag_context_, ppm_, search_form, scroll));
    return task->SyncStart();
  }

  // ---------------------------------------------------------------------------

  PushItem::Ptr AliOpenSearch::CreatePushItem(
    PushItemType type, const std::string &id){
    return PushItem::Ptr(new PushItem(type, id));
  }
  PushForm::Ptr AliOpenSearch::CreatePushForm(PushItem::Ptr push_item){
    return PushForm::Ptr(new PushForm(push_item));
  }

  // ---------------------------------------------------------------------------

  // Create ConfigStanza
  ConfigStanza::Ptr AliOpenSearch::CreateConfigStanza(){
    return ConfigStanza::Ptr(new ConfigStanza());
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
    SortType type, const std::string &sort_express){
    return SortStanza::Ptr(new SortStanza(type, sort_express));
  }

  AggregateStanza::Ptr AliOpenSearch::CreateAggregateStanza(
    const std::string &group_key, const std::string &agg_fun){
    return AggregateStanza::Ptr(new AggregateStanza(group_key, agg_fun));
  }

  DistinctStanza::Ptr AliOpenSearch::CreateDistinctStanza(
    const std::string &dist_key){
    return DistinctStanza::Ptr(new DistinctStanza(dist_key));
  }

  KvpairsStanza::Ptr AliOpenSearch::CreateKvpairsStanza(
    const std::string &key, const std::string value){
    return KvpairsStanza::Ptr(new KvpairsStanza(key, value));
  }

  Query::Ptr AliOpenSearch::CreateQuery(QueryStanza::Ptr query_stanza){
    return Query::Ptr(new Query(query_stanza));
  }

  Summary::Ptr AliOpenSearch::CreateSummary(const std::string &summary_field,
    const std::string &summary_element,
    const std::string &summary_ellipsis,
    int summary_snipped){
    return Summary::Ptr(new Summary(summary_field, summary_element,
      summary_ellipsis, summary_snipped));
  }

  SearchForm::Ptr AliOpenSearch::CreateSearchForm(
    Query::Ptr query, const std::string &app_name){
    return SearchForm::Ptr(new SearchForm(query, app_name));
  }

  Scroll::Ptr AliOpenSearch::CreateScroll(uint32 scroll_time,
    ScrollTimeType time_type,
    const std::string &search_type,
    const std::string &scroll_id){
    return Scroll::Ptr(new Scroll(
      scroll_time, time_type, search_type, scroll_id));
  }

}