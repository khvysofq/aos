/*
* libali_opensearch
* Copyright 2015 guangleihe@gmail.com.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http ://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "ali_search/ali_search.h"
#include "ali_search/req_task.h"
#include "base/logging.h"

//INITIALIZE_EASYLOGGINGPP


int main(int argv, char* argc[]) {

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
  cs->start();
  cs->hit();
  cs->format();
  cs->rerank_size();

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_config_stanza(cs);
  query->config_stanza();
  query->query_stanza();
  query->filter_stanza();
  query->sort_stanza();
  query->aggregate_stanza();
  query->distinct_stanza();
  query->kvpair_stanza();

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");
  search_form->query();
  search_form->index_app_names();
  search_form->fetch_fields();
  search_form->query_protos();
  search_form->first_formula_name();
  search_form->formula_name();
  search_form->summary();


  aos::Summary::Ptr summary = aosp->CreateSummary("id");
  summary->IsInvalueSummary();
  summary->summary_field();
  summary->summary_element();
  summary->summary_ellipsis();
  summary->summary_snipped();
  summary->summary_len();
  summary->summary_postfix();
  summary->summary_postfix();
  search_form->set_summary(summary);

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()) {
    LOG_INFO << "Search HELLO succeed";
    LOG_INFO << res_value->rep_json().toStyledString();
  } else {
    LOG_ERROR << "Get Error Log HELLO error"
              << res_value->GetErrorMessage()[0].message;
  }
  //getchar();
  //aos::QueryStanza::Ptr d = (a->)
  return 0;
}