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

  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::DistinctStanza::Ptr ds = aosp->CreateDistinctStanza("type_id");
  ds->set_dist_count(2);
  ds->set_dist_times(10);

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_distinct_stanza(ds);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << res_value->GetErrorMessage()[0].message;
  }
  //getchar();
  //aos::QueryStanza::Ptr d = (a->)
  return 0;
}