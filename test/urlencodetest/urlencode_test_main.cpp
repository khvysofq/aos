#include "ali_search/ali_search.h"
#include "base/basedefines.h"
#include "base/helpmethods.h"
#include "base/sha1.h"
#include <memory.h>
#include "base/base64.h"

//INITIALIZE_EASYLOGGINGPP

const std::string SIGN_STR = "GET&%2F&AccessKeyId%3Dtestid%26SignatureMethod"
"%3DHMAC-SHA1%26SignatureNonce%3D14053016951271226%26Si"
"gnatureVersion%3D1.0%26Timestamp%3D2014-07-14T01%253A3"
"4%253A55Z%26Version%3Dv2%26fetch_fields%3Dtitle%253Bgm"
"t_modified%26format%3Djson%26index_name%3Dut_3885312%2"
"6query%3Dconfig%253Dformat%253Ajson%252Cstart%253A0%25"
"2Chit%253A20%2526%2526query%253A%2527%25E7%259A%2584%2527";

const std::string KEY = "testsecret&";

const std::string B64_STR = "fxGidmIYSsx2AMa8onxuavOijuE=";

int main(int argv, char* argc[]){

  LOG(INFO) << SIGN_STR;

  unsigned char digest_buffer[SHA1_DIGEST_SIZE];

  aos::HelpMethos::HmacSha1((const unsigned char *)KEY.c_str(), KEY.size(), 
    (const unsigned char *)SIGN_STR.c_str(), SIGN_STR.size(), digest_buffer);

  for (int i = 0; i < SHA1_DIGEST_SIZE; i++){
    printf("%x", (unsigned char)digest_buffer[i]);
  }
  printf("\n");

  std::string result;
  std::size_t data_used = 0;
  aos::HelpMethos::HmacSha1ToBase64(KEY, SIGN_STR, result);
  LOG(INFO) << result;

  //aos::Base64::DecodeFromArray(B64_STR.c_str(), B64_STR.size(),
  //  aos::Base64::DecodeOption::DO_PARSE_STRICT, &result, &data_used);
  //const char *result_data = result.c_str();
  //for (std::size_t i = 0; i < result.size(); i++){
  //  printf("%x", (unsigned char)result_data[i]);
  //}
  //printf("\n");
  return 0;
}