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
  // Create app
  aos::ResValue::Ptr crs_value =
    aosp->CreateNewApp("HELLO_TEST", aos::APP_TEMPLATE_BUILTIN_NEWS);
  if (crs_value->IsSucceed()) {
    LOG_INFO << "Create app succeed " << "HELLO_TEST";
  } else {
    LOG_ERROR << "Create app failed "
              << crs_value->GetErrorMessage()[0].message;
  }
  // Get app status
  aos::ResValue::Ptr grs_value = aosp->GetAppStastus("HELLO_TEST");
  if (grs_value->IsSucceed()) {
    LOG_INFO << "Get app status succeed " << "HELLO";
  } else {
    LOG_ERROR << "get app status failed "
              << grs_value->GetErrorMessage()[0].message;
  }
  // Delete app
  aos::ResValue::Ptr drs_value = aosp->DeleteApp("HELLO_TEST");
  if (drs_value->IsSucceed()) {
    LOG_INFO << "Delete app succeed " << "HELLO";
  } else {
    LOG_ERROR << "Delete app failed "
              << drs_value->GetErrorMessage()[0].message;
  }
  // Delete warong app
  aos::ResValue::Ptr drsw_value = aosp->DeleteApp("HELLO_TEST2");
  if (drsw_value->IsSucceed()) {
    LOG_INFO << "Delete app succeed " << "HELLO_TEST2";
  } else {
    LOG_ERROR << "Delete app failed "
              << drsw_value->GetErrorMessage()[0].message;
  }
  return 0;
}