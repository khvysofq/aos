#ifndef ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_
#define ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_

#include "base/baseinclude.h"
#include "ali_search/public_part.h"
#include "ali_search/aoscontext.h"
#include "base/helpmethods.h"
#include "ali_search/res_value.h"
#include "ali_search/req_value.h"

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
  
  class SearchTask : public BaseReqTask, public noncopyable{
  public:

  private:
    SearchTask(AosGlobalContext::Ptr ag_context,
      PublicPartManager::Ptr ppmp, 
      SearchForm::Ptr search_form);
    friend class AliOpenSearch;
  private:
    SearchForm::Ptr search_form_;
  };
}

#endif // ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_