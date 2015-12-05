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

#include <string>
#include "ali_search/aoscontext.h"
#include "base/logging.h"


int main(int argv, char* argc[]) {
  aos::AosGlobalContext::Ptr agc =
    aos::AosGlobalContext::InitAosGlobalContext();

  if (!agc) {
    LOG_ERROR << "INIT AosGlobalContext Failed";
    return -1;
  }

  std::string req = "http://www.baidu.com";
  std::string rep;
  if (!agc->SyncProcessGetRequest(req, rep)) {
    LOG_ERROR << "SyncProcessGetRequest Falied";
    return -1;
  }
  LOG_INFO << rep;
  return 0;
}
