#include "base/helpmethods.h"
#include <time.h>
#include <stdio.h>
#include "timeutils.h"

namespace aos{

  const std::string HelpMethos::GetCurrentUTCTime(){
    
    // temp buffer
    static const int MAX_UTC_TIME_STR_SIZE = 32;
    char temp_buf[MAX_UTC_TIME_STR_SIZE];

    // Get current time
    time_t ct_seconds = time(NULL);
    tm *ct_tm = NULL;
#ifdef WIN32
    tm tt_tm;
    localtime_s(&tt_tm, &ct_seconds);
    ct_tm = &tt_tm;
    // YYYY-MM-DDThh:mm:ssZ
    sprintf_s(temp_buf, "%04d-%02d-%02dT%02d:%02d:%02dZ",
      ct_tm->tm_year + 1900,
      ct_tm->tm_mon + 1,
      ct_tm->tm_mday,
      ct_tm->tm_hour,
      ct_tm->tm_min,
      ct_tm->tm_sec);
#else
    ct_tm = localtime(&ct_seconds);
    // YYYY-MM-DDThh:mm:ssZ
    sprintf(temp_buf, "%04d-%02d-%02dT%02d:%02d:%02dZ",
      ct_tm->tm_year + 1900,
      ct_tm->tm_mon + 1,
      ct_tm->tm_mday,
      ct_tm->tm_hour,
      ct_tm->tm_min,
      ct_tm->tm_sec);
#endif
    return temp_buf;
  }

  const std::string HelpMethos::GetSignatureNonce(){
    std::string signature;
    Uint64ToString(time(NULL), signature);
    return signature + GetRandNumString(4);
  }

  void HelpMethos::Uint64ToString(uint64 n, std::string &str){
    if (n == 0){
      str.push_back('0');
      return;
    }
    while (n){
      str.push_back((n % 10) + '0');
      n = n / 10;
    }
  }

  const std::string HelpMethos::GetRandNumString(int size){
    static const int MAX_NUM_SET_SIZE = 10;
    std::string res_str;
    // Setting the random seed
    srand((unsigned int)TimeNanos());
    for (int i = 0; i < size; i++){
      res_str.push_back((rand() % MAX_NUM_SET_SIZE) + '0');
    }
    return res_str;
  }

}