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
  // Create app
  //aos::ResValue::Ptr crs_value =
  //  aosp->CreateNewApp("HELLO", aos::APP_TEMPLATE_BUILTIN_NEWS);
  //if (crs_value->IsSucceed()){
  //  LOG_INFO << "Create app succeed " << "HELLO";
  //}
  //else{
  //  LOG_ERROR << "Create app failed " 
  //    << crs_value->GetErrorMessage()[0].message;
  //}
  // Get app status
  aos::ResValue::Ptr grs_value = aosp->GetAppStastus("HELLO");
  if (grs_value->IsSucceed()){
    LOG_INFO << "Get app status succeed " << "HELLO";
  }
  else{
    LOG_ERROR << "get app status failed "
      << grs_value->GetErrorMessage()[0].message;
  }

  aos::PushItem::Ptr push_item1 = aosp->CreatePushItem(
    aos::PushItemType::ITEM_TYPE_ADD, "1");
  push_item1->AddField("type_id", "12");
  push_item1->AddField("cat_id", "13");
  push_item1->AddField("title", "test");
  push_item1->AddField("body", "This is a test, about something");
  push_item1->AddField("url", "www.baidu.com");

  aos::PushItem::Ptr push_item2 = aosp->CreatePushItem(
    aos::PushItemType::ITEM_TYPE_DELETE, "200");
  push_item2->AddField("type_id", "12");
  push_item2->AddField("cat_id", "13");
  push_item2->AddField("title", "test");
  push_item2->AddField("body", "This is a test, about something");
  push_item2->AddField("url", "www.baidu.com");


  aos::PushItem::Ptr push_item3 = aosp->CreatePushItem(
    aos::PushItemType::ITEM_TYPE_UPDATE, "3");
  push_item3->AddField("type_id", 12);
  push_item3->AddField("cat_id", 12.3);
  push_item3->AddField("title", "test");
  std::vector<int> bodys;
  bodys.push_back(1);
  bodys.push_back(2);
  bodys.push_back(3);
  bodys.push_back(4);
  push_item3->AddField("body", bodys);
  push_item3->AddField("url", "www.baidu.com");

  aos::PushForm::Ptr push_form = aosp->CreatePushForm(push_item1);
  push_form->AddPushItem(push_item2);
  push_form->AddPushItem(push_item3);

  aos::ResValue::Ptr res = aosp->PushIndexDoc("HELLO", "main", push_form);

  if (res->IsSucceed()){
    LOG_INFO << "Delete the HELLO main table succeed";
  }
  else{
    LOG_ERROR << "Delete the HELLO main table error"
      << res->GetErrorMessage()[0].message;
  }


  //// Delete app
  //aos::ResValue::Ptr drs_value = aosp->DeleteApp("HELLO");
  //if (drs_value->IsSucceed()){
  //  LOG_INFO << "Delete app succeed " << "HELLO";
  //}
  //else{
  //  LOG_ERROR << "Delete app failed " << drs_value->GetErrorMessage().message;
  //}

  return 0;
}