/*
* libali_opensearch
* Copyright 2015 guangleihe@gmail.com.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http ://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef SRC_BASE_HELPMETHODS_H_
#define SRC_BASE_HELPMETHODS_H_

#include <string>
#include "base/basictypes.h"

namespace aos {

enum HttpMethod {
  HTTP_GET,
  HTTP_POST
};

class HelpMethos {
 public:
  static const std::string GetCurrentUTCTime();
  static const std::string GetSignatureNonce();
  static void Uint64ToString(uint64 n, std::string &str); // NOLINT
  static const std::string GetRandNumString(int size);
  static uint32 GetUnixTimeStamp();
  static const std::string URLEncode(const std::string &str);

  static const std::string HttpMethodToStr(HttpMethod hm);

  static void HmacSha1(const uint8 *key, std::size_t key_size,
                       const uint8 *data, std::size_t data_size, uint8 *result);

  static void HmacSha1ToBase64(const std::string &key, // NOLINT
                               const std::string &data, // NOLINT
                               std::string &result); // NOLINT
};

}  // namespace aos
#endif  // SRC_BASE_HELPMETHODS_H_
