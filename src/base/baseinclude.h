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

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#ifndef SRC_BASE_BASEINCLUDE_H_
#define SRC_BASE_BASEINCLUDE_H_


// For std::shared_ptr, std::enable_shared_form_this
#include <memory>
// For std::mutex
#include <mutex> // NOLINT
#include <iostream>
#include <utility>

#include <set>
#include <map>
#include <string>

#include "base/noncopyable.h"
// With basic types
#include "base/basictypes.h"

namespace aos {

typedef std::map<std::string, std::string > KeyValues;
typedef std::pair<KeyValues::iterator, bool> KVSInsertResult;

}  // namespace aos


#endif  // SRC_BASE_BASEINCLUDE_H_
