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

#include "ali_search/res_value.h"
#include "base/common.h"
#include "base/basedefines.h"
#include "base/logging.h"

namespace aos {

ResValue::ResValue() :status_(false), search_time_(0) {
}

ResValue::~ResValue() {
}

bool ResValue::ParseResult() {
  if (rep_buffer_.empty()) {
    LOG_ERROR << "The rep buffer is null, you shouldn't parse the result";
    return false;
  }
  DLOG_INFO << rep_buffer_;
  // parse the json value
  Json::Reader rep_reader;
  if (!rep_reader.parse(rep_buffer_, json_res_)) {
    LOG_ERROR << rep_reader.getFormattedErrorMessages();
    return false;
  }
  return ParsePublicResult(json_res_);
}

bool ResValue::ParsePublicResult(Json::Value &json_res) {
  //
  if (json_res.isNull()) {
    LOG_ERROR << "The result is null";
    return false;
  }
  // Get the request id
  req_id_ = json_res[JSON_REQUEST_ID].asString();
  if (req_id_.empty()) {
    LOG_WARNING << "req_id is null";
  }
  if (!json_res[JSON_SEARCH_TIME].isNull()) {
    search_time_ = json_res[JSON_SEARCH_TIME].asDouble();
  }
  // Get result status
  std::string status_str = json_res[JSON_STATUS].asString();
  if (status_str.empty()) {
    status_str = JSON_STATUS_OK;
    LOG_WARNING << "the result status is empty";
  }
  if (status_str == JSON_STATUS_FALT) {
    status_ = false;
    ParseErrorMessage(json_res[JSON_ERRORS]);
    return true;
  } else if (status_str == JSON_STATUS_OK) {
    status_ = true;
    return ParsePrivateResult(json_res);
  } else {
    LOG_ERROR << "The result status is not the expectations value "
      << status_str;
    // ASSERT(false);
  }
  return false;
}

bool ResValue::ParseErrorMessage(Json::Value &error_res) {
  if (error_res.isNull()) {
    LOG_ERROR << "the error code filed is null";
    return false;
  }
  for (std::size_t i = 0; i < error_res.size(); i++) {
    AosError aos_error;
    aos_error.error_code = error_res[(int)i][JSON_ERRORS_CODE].asUInt();
    aos_error.message = error_res[(int)i][JSON_ERRORS_MESSAGE].asString();
    aos_errors_.push_back(aos_error);
    LOG_ERROR << "[ " << aos_error.error_code << " ] "
              << aos_error.message;
  }
  return true;
}

}  // namespace aos
