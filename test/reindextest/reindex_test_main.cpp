#include "ali_search/ali_search.h"
#include "ali_search/req_task.h"
#include "base/logging.h"

//INITIALIZE_EASYLOGGINGPP


int main(int argv, char* argc[]){

  aos::AosGlobalContext::Ptr agcp_context =
    aos::AosGlobalContext::InitAosGlobalContext();

  aos::AliOpenSearch::Ptr aosp = aos::AliOpenSearch::CreateAliOpenSearch(
    agcp_context,
    "http://opensearch-cn-beijing.aliyuncs.com",
    "635tekSIVXkFd9AL",
    "B1QP39FzM4I9bbudoF2Zxxmxk47fB9");

  aos::ResValue::Ptr res_value = aosp->ReindexApp("HELLO");
  if (res_value->IsSucceed()){
    LOG_INFO << "Reindex HELLO succeed";
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << "Reindex HELLO error"
      << res_value->GetErrorMessage()[0].message;
  }
  aosp->ReindexApp("HELLO","A");
  return 0;
}