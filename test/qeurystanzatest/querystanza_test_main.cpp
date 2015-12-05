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

  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("title", "北京大学");
  aos::QueryStanza::Ptr qb = aosp->CreateQueryStanza("title", "浙江大学");
  aos::QueryStanza::Ptr qc = aosp->CreateQueryStanza("type", "1");

  LOG_INFO << qa->Express();
  LOG_INFO << qb->Express();
  LOG_INFO << qc->Express();

  aos::QueryStanza::Ptr qd = qa->OrdinaryUnion(
                               aos::QueryUnionType::UNION_OR, qb);
  aos::QueryStanza::Ptr qe = qd->PriorityUnion(
                               aos::QueryUnionType::UNION_AND, qc);
  aos::QueryStanza::Ptr qf = qd->PriorityUnion(
                               aos::QueryUnionType::UNION_RANK, qc);

  LOG_INFO << qd->Express();
  LOG_INFO << qe->Express();

  aos::FilterStanza::Ptr fa = aosp->CreateFilterStanza("category=2");
  aos::FilterStanza::Ptr fb = aosp->CreateFilterStanza("create_time<140234560");
  aos::FilterStanza::Ptr fc = aosp->CreateFilterStanza("(hit+sale)*rate>10000");
  aos::FilterStanza::Ptr fd = fa->OrdinaryUnion(
                                aos::FilterUnionType::FILTER_UNION_TYPE_AND, fb);
  aos::FilterStanza::Ptr fe = fd->PriorityUnion(
                                aos::FilterUnionType::FILTER_UNION_TYPE_OR, fc);

  LOG_INFO << fa->Express();
  LOG_INFO << fb->Express();
  LOG_INFO << fc->Express();
  LOG_INFO << fd->Express();
  LOG_INFO << fe->Express();

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
  //aos::QueryStanza::Ptr d = (a->)
  return 0;
}