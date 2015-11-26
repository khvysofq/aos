﻿#include "ali_search/ali_search.h"
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

  aos::SortStanza::Ptr sa = aosp->CreateSortStanza(
    aos::SortType::ASC_TYPE, "RANK");
  aos::SortStanza::Ptr sb = aosp->CreateSortStanza(
    aos::SortType::ASC_TYPE, "(hits+comments)");
  aos::SortStanza::Ptr sc = aosp->CreateSortStanza(
    aos::SortType::ASC_TYPE, "type");
  aos::SortStanza::Ptr sd = sa->UnionSort(sb);
  aos::SortStanza::Ptr se = sd->UnionSort(sc);

  LOG_INFO << sa->Express();
  LOG_INFO << sb->Express();
  LOG_INFO << sc->Express();
  LOG_INFO << sd->Express();
  LOG_INFO << se->Express();

  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::SortStanza::Ptr ss = aosp->CreateSortStanza(aos::SortType::ASC_TYPE, "type_id");

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_sort_stanza(ss);

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