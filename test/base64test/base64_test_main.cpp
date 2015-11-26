#include "ali_search/ali_search.h"
#include "ali_search/req_task.h"
#include "base/logging.h"


int main(int argv, char* argc[]){
  
  aos::AosGlobalContext::Ptr agcp_context =
    aos::AosGlobalContext::InitAosGlobalContext();

  aos::AliOpenSearch::Ptr aosp = aos::AliOpenSearch::CreateAliOpenSearch(
    agcp_context,
    "http://opensearch-cn-beijing.aliyuncs.com",
    "635tekSIVXkFd9AL",
    "B1QP39FzM4I9bbudoF2Zxxmxk47fB9");

  aosp->CreateNewApp("HELLO", "game");

  return 0;
}