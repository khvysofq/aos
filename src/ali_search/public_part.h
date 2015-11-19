#ifndef ALI_OPENSEARCH_ALI_SEARCH_PUBLIC_PART_H_
#define ALI_OPENSEARCH_ALI_SEARCH_PUBLIC_PART_H_

#include "base/baseinclude.h"
#include "ali_search/keyvalue.h"
#include <set>

namespace aos{


  class PublicPartManager : public noncopyable,
    public std::enable_shared_from_this<PublicPartManager>{
  public:
    typedef std::shared_ptr<PublicPartManager> Ptr;
    PublicPartManager(const std::string &access_key_id,
      const std::string &access_key_secret);

    // Thread safe
    void CopyPublicPart(std::map<std::string, std::string> &kvs);
    virtual ~PublicPartManager();
  public:
    void InitPublicPart(const std::string &access_key_id,
      const std::string &access_key_secret);
  private:
    std::map<std::string, std::string> keyvalues_;
    std::mutex kvs_mutex_;
  };

} // namespace aos

#endif // ALI_OPENSEARCH_ALI_SEARCH_PUBLIC_PART_H_