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

#ifndef SRC_ALI_SEARCH_RES_VALUE_H_
#define SRC_ALI_SEARCH_RES_VALUE_H_

#include <string>
#include <vector>
#include "base/baseinclude.h"
#include "base/basictypes.h"
#include "json/json.h"

namespace aos {
//////////////////////////////////////////////////////////////////////////////
struct AosError {
  AosError() :error_code(0) {
  }
  uint32 error_code;
  std::string message;
};
typedef std::vector<AosError> AosErrors;

//////////////////////////////////////////////////////////////////////////////

class ResValue {
 public:
  typedef std::shared_ptr<ResValue> Ptr;
  ResValue();
  virtual ~ResValue();

  bool ParseResult();
  // public interface
  std::string &rep_buffer() {
    return rep_buffer_;
  }
  Json::Value &rep_json() {
    return json_res_;
  }
  bool IsSucceed() {
    return status_;
  }
  const AosErrors &GetErrorMessage() const {
    return aos_errors_;
  }
  const std::string &get_req_id() {
    return req_id_;
  }
  double search_time() {
    return search_time_;
  }

 private:
  bool ParsePublicResult(Json::Value &json_res);
  virtual bool ParsePrivateResult(Json::Value &json_res) {
    return true;
  }

  bool ParseErrorMessage(Json::Value &error_res);

 protected:
  std::string rep_buffer_;
  Json::Value json_res_;
  bool status_;
  AosErrors aos_errors_;
  std::string req_id_;
  double search_time_;
};
//////////////////////////////////////////////////////////////////////////////
}  // namespace aos

#endif  // SRC_ALI_SEARCH_RES_VALUE_H_
