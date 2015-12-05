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

#ifndef SRC_ALI_SEARCH_AOSCONTEXT_H_
#define SRC_ALI_SEARCH_AOSCONTEXT_H_

#include <string>
#include "base/baseinclude.h"

namespace aos {

class AosGlobalContext : public noncopyable,
  public std::enable_shared_from_this<AosGlobalContext> {
 public:
  typedef std::shared_ptr<AosGlobalContext> Ptr;
  virtual ~AosGlobalContext();

  // InitAosGlobalContext should be invoked exactly once for each
  // application that uses libali_opensearch and before any call of other
  // libali_opensearch functions
  // This function is not thread-safe
  static AosGlobalContext::Ptr InitAosGlobalContext();

  bool SyncProcessGetRequest(const std::string &url, std::string &rep);
  bool SyncProcessPostRequest(const std::string &url,
                              const std::string &data, std::string &rep);
 private:
  //
  AosGlobalContext();
  //
  bool InitLogginpp();
  bool InitAppResouce();

  static AosGlobalContext::Ptr agc_instance_;
};
}  // namespace aos

#endif  // SRC_ALI_SEARCH_AOSCONTEXT_H_
