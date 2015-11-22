#include "ali_search/ali_search.h"
#include "ali_search/req_task.h"

//INITIALIZE_EASYLOGGINGPP


int main(int argv, char* argc[]){

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
  
  LOG_INFO << qa->value();
  LOG_INFO << qb->value();
  LOG_INFO << qc->value();

  aos::QueryStanza::Ptr qd = qa->OrdinaryUnion(
    aos::QueryUnionType::UNION_OR, qb);
  aos::QueryStanza::Ptr qe = qd->PriorityUnion(
    aos::QueryUnionType::UNION_AND, qc);
  aos::QueryStanza::Ptr qf = qd->PriorityUnion(
    aos::QueryUnionType::UNION_RANK, qc);

  LOG_INFO << qd->value();
  LOG_INFO << qe->value();

  aos::FilterStanza::Ptr fa = aosp->CreateFilterStanza("category=2");
  aos::FilterStanza::Ptr fb = aosp->CreateFilterStanza("create_time<140234560");
  aos::FilterStanza::Ptr fc = aosp->CreateFilterStanza("(hit+sale)*rate>10000");
  aos::FilterStanza::Ptr fd = fa->OrdinaryUnion(
    aos::FilterUnionType::FILTER_UNION_TYPE_AND, fb);
  aos::FilterStanza::Ptr fe = fd->PriorityUnion(
    aos::FilterUnionType::FILTER_UNION_TYPE_OR, fc);

  LOG_INFO << fa->filter_express();
  LOG_INFO << fb->filter_express();
  LOG_INFO << fc->filter_express();
  LOG_INFO << fd->filter_express();
  LOG_INFO << fe->filter_express();

  aos::SortStanza::Ptr sa = aosp->CreateSortStanza("+RANK");
  aos::SortStanza::Ptr sb = aosp->CreateSortStanza("-(hits+comments)");
  aos::SortStanza::Ptr sc = aosp->CreateSortStanza("+type");
  aos::SortStanza::Ptr sd = sa->UnionFilter(sb);
  aos::SortStanza::Ptr se = sd->UnionFilter(sc);

  LOG_INFO << sa->sort_express();
  LOG_INFO << sb->sort_express();
  LOG_INFO << sc->sort_express();
  LOG_INFO << sd->sort_express();
  LOG_INFO << se->sort_express();

  getchar();
  //aos::QueryStanza::Ptr d = (a->)
  return 0;
}