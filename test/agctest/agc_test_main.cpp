
#include "ali_search/aoscontext.h"


int main(int argv, char* argc[]){

  aos::AosGlobalContext::Ptr agc = aos::AosGlobalContext::InitAosGlobalContext();

  if (!agc){
    LOG_ERROR << "INIT AosGlobalContext Failed";
    return -1;
  }

  std::string req = "http://www.baidu.com";
  std::string rep;
  if (!agc->SyncProcessGetRequest(req, rep)){
    LOG_ERROR << "SyncProcessGetRequest Falied";
    return -1;
  }
  LOG_INFO << rep;
  return 0;
}