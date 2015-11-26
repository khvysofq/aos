#include "ali_search/ali_search.h"
#include "base/basedefines.h"
#include "base/helpmethods.h"
#include "base/logging.h"
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

const std::string URL_TEST_01 = "µÄVersion=v2&AccessKeyId=testid&SignatureMethod=HMAC£­SHA1&SignatureVersion=1.0&SignatureNonce=14053016951271226&Timestamp=2014-07-14T01:34:55Z";

char CN[] = "µÄ";

const std::string TIME_UTC_STR = "::::::::::";

int main(int argv, char* argc[]){
  
  for (int i = 0; i < 10; i++){
    LOG_INFO << aos::HelpMethos::GetCurrentUTCTime();
  }

  for (int i = 0; i < 10; i++){
    LOG_INFO << aos::HelpMethos::GetSignatureNonce();
  }

  LOG_INFO << CN;

  LOG_INFO << aos::HelpMethos::URLEncode(URL_TEST_01);

  LOG_INFO << aos::HelpMethos::URLEncode(TIME_UTC_STR);

  return 0;
}