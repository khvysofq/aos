#ifndef ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_
#define ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_

#include "base/baseinclude.h"
#include "ali_search/public_part.h"
#include "ali_search/aoscontext.h"
#include "base/helpmethods.h"
#include "ali_search/res_value.h"

namespace aos{

  //enum HttpMethod {
  //  HTTP_GET,
  //  HTTP_POST
  //};

  class BaseReqTask{
  public:
    explicit BaseReqTask(AosGlobalContext::Ptr ag_context, 
      PublicPartManager::Ptr ppmp, 
      const std::string &api_url, 
      HttpMethod http_method);
    virtual ResValue::Ptr SyncStart();
    ResValue::Ptr res_value(){ return res_value_; }
    virtual ~BaseReqTask();
  protected:
    virtual void BuildReqUrl(std::string &result);
    virtual void BuildRequestParames(std::string &parames);
    virtual bool BuildPostData(std::string &post_data){ return false; }

    virtual bool AddKeyValue(const std::string &key, const std::string &value);
    virtual void AddApiUrl(std::string &result) = 0;
    virtual void AddPrivateKeyValues() = 0;
    virtual ResValue *GetResBuffer() = 0;

    void GeneratorCCQS(std::string &ccqs);
    const std::string GeneratorSignature(const std::string &cqs);

  protected:

    PublicPartManager::Ptr ppmp_;
    AosGlobalContext::Ptr ag_context_;
    KeyValues keyvalues_;
    HttpMethod http_method_;
    std::string api_url_;
    ResValue::Ptr res_value_;
  };

  //////////////////////////////////////////////////////////////////////////////
  class CreateAppTask : public BaseReqTask, public noncopyable{
  public:
    typedef std::shared_ptr<CreateAppTask> Ptr;
    virtual ~CreateAppTask();
    ResValue &GetResultValue();
  private:
    explicit CreateAppTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp,
      const std::string &app_name,
      const std::string &template_name);

    virtual void AddApiUrl(std::string &result);
    virtual void AddPrivateKeyValues();
    virtual ResValue *GetResBuffer(){
      return res_value_.get();
    }

    friend class AliOpenSearch;
  private:
    std::string app_name_;
    std::string template_name_;
  };

  //////////////////////////////////////////////////////////////////////////////
  class GetAppStatusTask : public BaseReqTask, public noncopyable{
  public:
    typedef std::shared_ptr<GetAppStatusTask> Ptr;
    virtual ~GetAppStatusTask();
  private:
    explicit GetAppStatusTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp,
      const std::string &app_name);

    virtual void AddApiUrl(std::string &result);
    virtual void AddPrivateKeyValues();
    virtual ResValue *GetResBuffer(){
      return res_value_.get();
    }

    friend class AliOpenSearch;
  private:
    std::string app_name_;
  };

  //////////////////////////////////////////////////////////////////////////////
  class DeleteAppTask : public BaseReqTask, public noncopyable{
  public:
    typedef std::shared_ptr<DeleteAppTask> Ptr;
    virtual ~DeleteAppTask();
  private:
    explicit DeleteAppTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp,
      const std::string &app_name);

    virtual void AddApiUrl(std::string &result);
    virtual void AddPrivateKeyValues();
    virtual ResValue *GetResBuffer(){
      return res_value_.get();
    }

    friend class AliOpenSearch;
  private:
    std::string app_name_;
  };

  //////////////////////////////////////////////////////////////////////////////
  enum PushItemType{
    ITEM_TYPE_ADD,
    ITEM_TYPE_UPDATE,
    ITEM_TYPE_DELETE
  };

  // ---------------------------------------------------------------------------
  class PushItem :public noncopyable{
  public:
    typedef std::shared_ptr<PushItem> Ptr;

    const std::string &type() const { return type_; }
    const std::string &id() const { return id_; }
    const Json::Value &fields() const { return fields_json_; }

    bool AddField(const std::string &key, const std::string &value);
    bool AddField(const std::string &key, Json::Value &value);

  private:
    friend class PushIndexDocTask;
    PushItem(PushItemType type, const std::string &id);
    const std::string PushItemTypeToString(PushItemType type);

    Json::Value &ToJson();
  private:
    std::string type_;
    std::string id_;
    Json::Value item_json_;
    Json::Value fields_json_;
  };
  // ---------------------------------------------------------------------------

  class PushIndexDocTask : public BaseReqTask, public noncopyable{
  public:
    typedef std::shared_ptr<PushIndexDocTask> Ptr;
    virtual ~PushIndexDocTask();

    PushItem::Ptr AddItem(PushItemType type, const std::string &id);

  private:
    explicit PushIndexDocTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp,
      const std::string &app_name, 
      const std::string &table_name);

    virtual bool BuildPostData(std::string &post_data);

    virtual void AddApiUrl(std::string &result);
    virtual void AddPrivateKeyValues();
    virtual ResValue *GetResBuffer(){
      return res_value_.get();
    }

    friend class AliOpenSearch;
  private:
    std::string app_name_;
    std::string table_name_;
    std::vector<PushItem::Ptr> items_;
  };

  //////////////////////////////////////////////////////////////////////////////
  class SuggestTask : public BaseReqTask, public noncopyable{
  public:
    typedef std::shared_ptr<SuggestTask> Ptr;
    virtual ~SuggestTask();
  private:
    explicit SuggestTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp,
      const std::string &app_name,
      const std::string &query,
      const std::string &suggest_name,
      int hit_count);

    virtual void AddApiUrl(std::string &result);
    virtual void AddPrivateKeyValues();
    virtual ResValue *GetResBuffer(){
      return res_value_.get();
    }

    friend class AliOpenSearch;
  private:
    std::string app_name_;
    std::string suggest_name_;
    std::string query_;
    int hit_count_;
  };
  //////////////////////////////////////////////////////////////////////////////
  class ReindexTask : public BaseReqTask, public noncopyable{
  public:
    typedef std::shared_ptr<ReindexTask> Ptr;
    virtual ~ReindexTask();
  private:
    explicit ReindexTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp,
      const std::string &app_name,
      const std::string &operate,
      const std::string &table_name);

    virtual void AddApiUrl(std::string &result);
    virtual void AddPrivateKeyValues();
    virtual ResValue *GetResBuffer(){
      return res_value_.get();
    }
    friend class AliOpenSearch;
  private:
    std::string app_name_;
    std::string operate_;
    std::string table_name_;
  };

  //////////////////////////////////////////////////////////////////////////////
  enum SortMode{
    SORT_ASC,
    SORT_DESC
  };
  class GetErrorLogTask : public BaseReqTask, public noncopyable{
  public:
    typedef std::shared_ptr<GetErrorLogTask> Ptr;
    virtual ~GetErrorLogTask();
  private:
    explicit GetErrorLogTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp,
      const std::string &app_name,
      int page, int page_size, SortMode sort_mode);

    virtual void AddApiUrl(std::string &result);
    virtual void AddPrivateKeyValues();
    virtual ResValue *GetResBuffer(){
      return res_value_.get();
    }
    const std::string SortModeToString(SortMode sort_mode);
    friend class AliOpenSearch;
  private:
    std::string app_name_;
    int page_;
    int page_size_;
    SortMode sort_mode_;
  };

  //////////////////////////////////////////////////////////////////////////////
  // ---------------------------------------------------------------------------
  enum QueryUnionType{
    UNION_AND,
    UNION_OR,
    UNION_ANDNOT,
    UNION_RANK
  };

  class QueryStanza : public noncopyable{
  public:
    typedef std::shared_ptr<QueryStanza> Ptr;

    QueryStanza::Ptr OrdinaryUnion(QueryUnionType type,
      QueryStanza::Ptr query_stanza);
    QueryStanza::Ptr PriorityUnion(QueryUnionType type,
      QueryStanza::Ptr query_stanza);

    const std::string &value() const{ return value_; }
  private:
    QueryStanza(const std::string &key_word,
      const std::string &index_name);
    QueryStanza(const std::string &value);
    const std::string UnionTypeToString(QueryUnionType type);
    friend class AliOpenSearch;
  private:
    std::string value_;
  };
  // ---------------------------------------------------------------------------

  // ---------------------------------------------------------------------------
  enum FilterUnionType{
    FILTER_UNION_TYPE_AND,
    FILTER_UNION_TYPE_OR
  };

  class FilterStanza : public noncopyable{
  public:
    typedef std::shared_ptr<FilterStanza> Ptr;

    FilterStanza::Ptr OrdinaryUnion(FilterUnionType type,
      FilterStanza::Ptr filter_stanza);
    FilterStanza::Ptr PriorityUnion(FilterUnionType type,
      FilterStanza::Ptr filter_stanza);

    const std::string &filter_express() const{ return filter_express_; }
  private:
    FilterStanza(const std::string &filter_express);
    const std::string FilterUnionTypeToString(FilterUnionType type);
    friend class AliOpenSearch;
  private:
    std::string filter_express_;
  };

  // ---------------------------------------------------------------------------
  class SortStanza : public noncopyable{
  public:
    typedef std::shared_ptr<SortStanza> Ptr;

    SortStanza::Ptr UnionFilter(SortStanza::Ptr sort_stanza);

    const std::string &sort_express() const{ return sort_express_; }
  private:
    SortStanza(const std::string &sort_express);
    friend class AliOpenSearch;
  private:
    std::string sort_express_;
  };

  // ---------------------------------------------------------------------------
  class AggregateStanza : public noncopyable{
  public:

    const std::string &aggregate_express();

    // about the range
    void AddAggRange(uint32 min, uint32 max);
    void ClearRange();

    // 
    void AddAggFunc(const std::string &func_express);
    void ClearAggFunc();

    // filter
    void set_agg_filter(const std::string &agg_filter){
      agg_filter_ = agg_filter;
    }

    // 
    void set_max_group(uint32 max_group){
      max_group_ = max_group;
    }
    void set_agg_sampler_threshold(uint32 agg_sampler_threshold){
      agg_sampler_threshold_ = agg_sampler_threshold;
    }
    void set_agg_sampler_step(uint32 agg_sampler_step){
      agg_sampler_step_ = agg_sampler_step;
    }

  private:
    AggregateStanza(const std::string &group_key);

  private:
    std::string group_key_;
    std::set<std::string> ranges_;
    std::set<std::string> agg_funcs_;
    std::string agg_filter_;
    uint32 max_group_;
    uint32 agg_sampler_threshold_;
    uint32 agg_sampler_step_;
  };

  // ---------------------------------------------------------------------------

  class SearchQuery : public noncopyable{
  public:
    void set_query_stanza(QueryStanza::Ptr query_stanza);
  private:
    QueryStanza::Ptr query_stanza_;
  };

  class SearchSummary : public noncopyable{
  public:
    typedef std::shared_ptr<SearchSummary> Ptr;

    bool ToSummaryString(std::string &summary);

    bool IsInvalueSummary(){ return summary_field_.empty(); }

    // summary_field
    const std::string &summary_field() const { return summary_field_; }
    bool set_summary_filed(const std::string &summary_filed);

    // summary_element
    const std::string &summary_element() const { return summary_element_; }
    bool set_summary_element(const std::string &summary_element);

    // summary ellipsis
    const std::string &summary_ellipsis() const{ return summary_ellipsis_; };
    bool set_summary_ellipsis(const std::string &summary_ellipsis);

    // summary snipped
    uint32 summary_snipped() const { return summary_snipped_; }
    bool set_summary_snipped(uint32 summary_snipped);

    // summary len
    uint32 summary_len() const { return summary_len_; }
    bool set_summary_len(uint32 summary_len);

    // summary prefix and summary postfix
    const std::string &summary_prefix() const { return summary_prefix_; }
    const std::string &summary_postfix() const { return summary_postfix_; }
    bool set_summary_prefix_postfix(
      const std::string &summary_prefix, const std::string &summary_postfix);

  private:
    SearchSummary(const std::string &summary_field,
      const std::string &summary_element,
      const std::string &summary_ellipsis,
      int summary_snipped);

    void AddKeyValueToString(std::string &str,
      const std::string &key, const std::string &value);

    friend class SearchForm;
  private:
    std::string summary_field_;
    std::string summary_element_;
    std::string summary_ellipsis_;
    uint32 summary_snipped_;
    uint32 summary_len_;
    std::string summary_prefix_;
    std::string summary_postfix_;
  };

  class SearchForm : public noncopyable{
  public:
    typedef std::shared_ptr<SearchForm> Ptr;
    // about index name
    void AddSearchAppName(const std::string &app_name);
    void RemoveSearchAppName(const std::string &app_name);
    // fetch fileds
    void AddFetchFields(const std::string &app_name);
    void RemoveFetchFields(const std::string &app_name);
    // about qp
    void AddQueryProto(const std::string &proto);
    void RemoveQueryProto(const std::string &proto);
    // about summary
    SearchSummary::Ptr GetCurrentSummary(){ return summary_; }
    SearchSummary::Ptr CreateAndEnableSummary(
      const std::string &summary_field,
      const std::string &summary_element = "em",
      const std::string &summary_ellipsis = "...",
      int summary_snipped = 1);
    bool EnableSummary();
    void DisableSummary();
  private:
    SearchForm();
  private:
    std::set<std::string> index_app_names_;
    std::set<std::string> fetch_fields_;
    std::set<std::string> query_proto_;
    std::string disable_;
    std::string first_formula_name_;
    std::string formula_name_;
    SearchSummary::Ptr summary_;
    bool enable_summary_;
  };

  class SearchTask : public BaseReqTask, public noncopyable{
  public:

  private:
  };
}

#endif // ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_