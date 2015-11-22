#ifndef ALI_OPENSEARCH_AL_OPENSEARCH_AOS_CONTEXT_H_
#define ALI_OPENSEARCH_AL_OPENSEARCH_AOS_CONTEXT_H_

#include "base/baseinclude.h"

namespace aos{

  class AosGlobalContext : public noncopyable,
    public std::enable_shared_from_this<AosGlobalContext>{
  public:
    typedef std::shared_ptr<AosGlobalContext> Ptr;
    virtual ~AosGlobalContext();

    // InitAosGlobalContext should be invoked exactly once for each
    // application that uses libali_opensearch and before any call of other
    // libali_opensearch functions
    // This function is not thread-safe
    static AosGlobalContext::Ptr InitAosGlobalContext();

    bool SyncProcessGetRequest(const std::string &url, std::string &rep);
    bool SyncProcessPostRequest(const std::string &url, 
      const std::string &data, std::string &rep);
  private:
    //
    AosGlobalContext();
    // 
    bool InitLogginpp();
    bool InitAppResouce();

    static AosGlobalContext::Ptr agc_instance_;

  };

} // namespace aos

#endif // ALI_OPENSEARCH_AL_OPENSEARCH_AOS_CONTEXT_H_