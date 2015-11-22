﻿#include "ali_search/ali_search.h"
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

  aos::PushIndexDocTask::Ptr push_task =
    aosp->BuildPushIndexDocTask("HELLO", "main");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "1");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "2");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "3");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "4");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "5");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "6");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "7");
  push_task->AddItem(aos::PushItemType::ITEM_TYPE_DELETE, "8");
  aos::ResValue::Ptr pid_value = push_task->SyncStart();
  if (grs_value->IsSucceed()){
    LOG_INFO << "Delete the HELLO main table succeed";
  }
  else{
    LOG_ERROR << "Delete the HELLO main table error"
      << grs_value->GetErrorMessage()[0].message;
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