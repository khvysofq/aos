
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#ifndef ALI_OPENSEARCH_BASE_BASEINCLUDE_H_
#define ALI_OPENSEARCH_BASE_BASEINCLUDE_H_


// for std::shared_ptr, std::sconprt, std::enable_shared_form_this
#include <memory>
// For std::mutex
#include <mutex>
#include <iostream>

#include "base/noncopyable.h"

// for logging library

//#define EASY_LOGGING_CPP

#ifdef EASY_LOGGING_CPP
#include "easylogging++.h"
#endif

#define GOOGLE_GLOG_LIBRARY
#ifdef GOOGLE_GLOG_LIBRARY
#include "glog/logging.h"
#endif

#include <set>
#include <map>

// With basic types
#include "base/basictypes.h"

namespace aos{

  typedef std::map<std::string, std::string > KeyValues;
  typedef std::pair<KeyValues::iterator, bool> KVSInsertResult;

#ifdef GOOGLE_GLOG_LIBRARY

#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_ERROR LOG(ERROR)

#define DLOG_INFO DLOG(INFO)
#define DLOG_WARNING DLOG(WARNING)
#define DLOG_ERROR DLOG(WARNING)
#else

#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_ERROR LOG(ERROR)

#define DLOG_INFO DLOG(INFO)
#define DLOG_WARNING DLOG(WARNING)
#define DLOG_ERROR DLOG(WARNING)
#endif
}


#endif // ALI_OPENSEARCH_BASE_BASEINCLUDE_H_