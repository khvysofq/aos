#ifndef ALI_OPENSEARCH_AL_OPENSEARCH_AL_OPENSEARCH_H_
#define ALI_OPENSEARCH_AL_OPENSEARCH_AL_OPENSEARCH_H_

#include "curl/curl.h"
#include "base/baseinclude.h"
#include "ali_search/public_part.h"

namespace aos{
  class AliOpenSearch : public noncopyable, 
    public std::enable_shared_from_this<AliOpenSearch>{
  public:
    typedef std::shared_ptr<AliOpenSearch> Ptr;

    static AliOpenSearch::Ptr CreateAliOpenSearch(
      const std::string &access_key_id,
      const std::string &access_key_secret);
    virtual ~AliOpenSearch();

    // Public Method
    void CreateNewApp(const std::string &app_name, 
      const std::string &template_name);

  private:
    // AliOpenSearch::InitAliSearch should be invoked exactly once for each
    // application that uses libali_opensearch and before any call of other
    // libali_opensearch functions
    // This function is not thread-safe
    // bool InitAliSearch();
    AliOpenSearch(const std::string &access_key_id,
      const std::string &access_key_secret);
  private:
    PublicPartManager::Ptr ppm_;
  };
}

#endif // ALI_OPENSEARCH_AL_OPENSEARCH_H_