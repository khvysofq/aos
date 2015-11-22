#include "ali_search/ali_search.h"
#include "ali_search/req_task.h"

//INITIALIZE_EASYLOGGINGPP


int main(int argv, char* argc[]){
  
  aos::AosGlobalContext::Ptr agcp_context =
    aos::AosGlobalContext::InitAosGlobalContext();

  aos::AliOpenSearch::Ptr aosp = aos::AliOpenSearch::CreateAliOpenSearch(
    agcp_context, "testid", "testsecret&");

  aosp->CreateNewApp("HELLO", "game");

  return 0;
}