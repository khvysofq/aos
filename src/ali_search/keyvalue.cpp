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

#include "ali_search/keyvalue.h"
#include <string>
#include "base/logging.h"

namespace aos {

KeyValue::KeyValue(const std::string &key, const std::string &value)
  :key_(key), value_(value) {
}

bool KeyValue::operator<(const KeyValue &kv) const {
  if (key_ < kv.key()) {
    return true;
  }
  if (key_ == kv.key() && value_ < kv.value()) {
    return true;
  }
  return false;
}
}  // namespace aos
