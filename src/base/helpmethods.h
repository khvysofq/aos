#ifndef ALI_OPENSEARCH_BASE_HELP_METHODS_H_
#define ALI_OPENSEARCH_BASE_HELP_METHODS_H_

#include <string>
#include "base/basictypes.h"

namespace aos{

  class HelpMethos{
  public:
    static const std::string GetCurrentUTCTime();
    static const std::string GetSignatureNonce();
    static void Uint64ToString(uint64 n, std::string &str);
    static const std::string GetRandNumString(int size);
  };

}

#endif // ALI_OPENSEARCH_BASE_HELP_METHODS_H_