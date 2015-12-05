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

#ifndef SRC_BASE_LOGGING_H_
#define SRC_BASE_LOGGING_H_

#include <iostream>

// #define EASY_LOGGING_CPP
#ifdef EASY_LOGGING_CPP

#include "easylogging/easylogging++.h"
#endif

// #define GOOGLE_GLOG_LIBRARY
#ifdef GOOGLE_GLOG_LIBRARY
#include "glog/logging.h"
#endif

namespace aos {

#ifdef GOOGLE_GLOG_LIBRARY

#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_ERROR LOG(ERROR)

#define DLOG_INFO DLOG(INFO)
#define DLOG_WARNING DLOG(WARNING)
#define DLOG_ERROR DLOG(WARNING)
#else

#define LOG_INFO std::cout
#define LOG_WARNING std::cout
#define LOG_ERROR std::cout

#define DLOG_INFO std::cout
#define DLOG_WARNING std::cout
#define DLOG_ERROR std::cout
#endif
}  // namespace aos


#endif  // SRC_BASE_LOGGING_H_
