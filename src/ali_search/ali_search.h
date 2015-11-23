#ifndef ALI_OPENSEARCH_AL_OPENSEARCH_AL_OPENSEARCH_H_
#define ALI_OPENSEARCH_AL_OPENSEARCH_AL_OPENSEARCH_H_

#include "curl/curl.h"
#include "base/baseinclude.h"
#include "base/basedefines.h"
#include "ali_search/public_part.h"
#include "ali_search/aoscontext.h"
#include "ali_search/res_value.h"
#include "ali_search/req_task.h"

namespace aos{
  class AliOpenSearch : public noncopyable, 
    public std::enable_shared_from_this<AliOpenSearch>{
  public:
    typedef std::shared_ptr<AliOpenSearch> Ptr;
    virtual ~AliOpenSearch();

    static AliOpenSearch::Ptr CreateAliOpenSearch(
      AosGlobalContext::Ptr ag_context,
      const std::string &api_domain,
      const std::string &access_key_id,
      const std::string &access_key_secret);

    // Public Method
    //const char APP_TEMPLATE_BUILTIN_NEWS[] = "builtin_news";
    //const char APP_TEMPLATE_BUILTIN_NOVEL[] = "builtin_novel";
    //const char APP_TEMPLATE_BUILTIN_DOWNLOAD[] = "builtin_download";
    //const char APP_TEMPLATE_BUILTIN_BBS[] = "bbs";
    ResValue::Ptr CreateNewApp(const std::string &app_name, 
      const std::string &template_name);
    //
    ResValue::Ptr GetAppStastus(const std::string &app_name);
    //
    ResValue::Ptr DeleteApp(const std::string &app_name);

    //
    PushIndexDocTask::Ptr BuildPushIndexDocTask(const std::string &app_name,
      const std::string &table_name);

    //
    ResValue::Ptr SuggestHit(const std::string &app_name,
      const std::string &query, const std::string &suggest_name,
      int hit_count = 10);

    //
    ResValue::Ptr ReindexApp(const std::string &app_name,
      const std::string &operate = "",
      const std::string &table_name = "");

    //
    ResValue::Ptr GetAppErrorLog(const std::string &app_name,
      int page, int page_size, SortMode sort_mode);

    // Create ConfigStanza
    ConfigStanza::Ptr CreateConfigStanza();
    // Create QueryStanza
    QueryStanza::Ptr CreateQueryStanza(const std::string &index_name,
      const std::string &key_word);
    // Create FilterStanza
    FilterStanza::Ptr CreateFilterStanza(const std::string &filter_express);
    // Create SortStanza
    SortStanza::Ptr CreateSortStanza(const std::string &sort_express);
    // Create AggregateStanza
    AggregateStanza::Ptr CreateAggregateStanza(const std::string &group_key);
    // Create DistinctStanza
    DistinctStanza::Ptr CreateDistinctStanza(const std::string &dist_key);
    // Create KvpairsStanza
    KvpairsStanza::Ptr CreateKvpairsStanza(
      const std::string &key, const std::string value);
    
    // Create Query
    Query::Ptr CreateQuery(QueryStanza::Ptr query_stanza);
    // Create Summary
    Summary::Ptr CreateSummary(
      const std::string &summary_field,
      const std::string &summary_element = "em",
      const std::string &summary_ellipsis = "...",
      int summary_snipped = 1);
  private:
    AliOpenSearch(AosGlobalContext::Ptr ag_context,
      const std::string &api_domain,
      const std::string &access_key_id,
      const std::string &access_key_secret);
  private:
    PublicPartManager::Ptr ppm_;
    AosGlobalContext::Ptr ag_context_;
  };
}

#endif // ALI_OPENSEARCH_AL_OPENSEARCH_H_