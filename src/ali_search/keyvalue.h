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


#ifndef SRC_ALI_SEARCH_KEYVALUE_H_
#define SRC_ALI_SEARCH_KEYVALUE_H_

#include <string>
#include "base/baseinclude.h"

namespace aos {

class KeyValue {
 public:
  explicit KeyValue(const std::string &key, const std::string &value);

  const std::string &key() const {
    return key_;
  }
  const std::string &value() const {
    return value_;
  }

  void set_key(const std::string &key) {
    key_ = key;
  }
  void set_value(const std::string &value) {
    value_ = value;
  }

  bool operator<(const KeyValue &kv) const;
 public:
  std::string key_;
  std::string value_;
};
}  // namespace aos

#endif  // SRC_ALI_SEARCH_KEYVALUE_H_
