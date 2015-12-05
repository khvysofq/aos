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

#include "ali_search/ali_search.h"
#include "base/basedefines.h"
#include "base/helpmethods.h"
#include "base/sha1.h"
#include <memory.h>
#include "base/logging.h"

//INITIALIZE_EASYLOGGINGPP

const std::string SIGN_STR
  = "GET&%2F&AccessKeyId%3Dtestid%26SignatureMethod"
    "%3DHMAC-SHA1%26SignatureNonce%3D14053016951271226%26Si"
    "gnatureVersion%3D1.0%26Timestamp%3D2014-07-14T01%253A3"
    "4%253A55Z%26Version%3Dv2%26fetch_fields%3Dtitle%253Bgm"
    "t_modified%26format%3Djson%26index_name%3Dut_3885312%2"
    "6query%3Dconfig%253Dformat%253Ajson%252Cstart%253A0%25"
    "2Chit%253A20%2526%2526query%253A%2527%25E7%259A%2584%2527";

const std::string KEY = "testsecret&";


int main(int argv, char* argc[]) {

  LOG_INFO << SIGN_STR;

  unsigned char digest_buffer[SHA1_DIGEST_SIZE];

  aos::HelpMethos::HmacSha1((const unsigned char *)KEY.c_str(), KEY.size(),
                            (const unsigned char *)SIGN_STR.c_str(), SIGN_STR.size(), digest_buffer);

  for (int i = 0; i < SHA1_DIGEST_SIZE; i++) {
    printf("%x", (unsigned char)digest_buffer[i]);
  }
  printf("\n");

  std::string result;
  std::size_t data_used = 0;
  aos::HelpMethos::HmacSha1ToBase64(KEY, SIGN_STR, result);
  LOG_INFO << result;


  return 0;
}