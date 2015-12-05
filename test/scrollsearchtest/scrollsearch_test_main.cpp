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
  aos::ConfigStanza::Ptr cs = aosp->CreateConfigStanza();
  cs->set_hit(1);

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_config_stanza(cs);

  aos::Scroll::Ptr scroll = aosp->CreateScroll(2);
  scroll->scroll_time();
  scroll->time_type();
  scroll->scroll_id();

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->ScrollSearch(search_form, scroll);
  if (res_value->IsSucceed()){
    LOG_INFO << "Search HELLO succeed";
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << "Get Error Log HELLO error"
      << res_value->GetErrorMessage()[0].message;
  }
  aosp->ScrollSearch(search_form, scroll);
  LOG_INFO << scroll->scroll_id();
  //getchar();
  //aos::QueryStanza::Ptr d = (a->)
  return 0;
}