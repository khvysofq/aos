#ifndef ALI_OPENSEARCH_BASE_HELP_METHODS_H_
#define ALI_OPENSEARCH_BASE_HELP_METHODS_H_

#include <string>
#include "base/basictypes.h"

namespace aos{

  enum HttpMethod {
    HTTP_GET,
    HTTP_POST
  };

  class HelpMethos{
  public:
    static const std::string GetCurrentUTCTime();
    static const std::string GetSignatureNonce();
    static void Uint64ToString(uint64 n, std::string &str);
    static const std::string GetRandNumString(int size);
    static uint32 GetUnixTimeStamp();
    static const std::string URLEncode(const std::string &str);

    static const std::string HttpMethodToStr(HttpMethod hm);

    static void HmacSha1(const uint8 *key, std::size_t key_size,
      const uint8 *data, std::size_t data_size, uint8 *result);

    static void HmacSha1ToBase64(const std::string &key,
      const std::string &data, std::string &result);
  };

}

#endif // ALI_OPENSEARCH_BASE_HELP_METHODS_H_