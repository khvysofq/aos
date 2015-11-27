#ifndef ALI_OPENSEARCH_BASE_LOGGING_H_
#define ALI_OPENSEARCH_BASE_LOGGING_H_

//#define EASY_LOGGING_CPP
#ifdef EASY_LOGGING_CPP
#include "easylogging++.h"
#endif

//#define GOOGLE_GLOG_LIBRARY
#ifdef GOOGLE_GLOG_LIBRARY
#include "glog/logging.h"
#endif

namespace aos{

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
}


#endif // ALI_OPENSEARCH_BASE_LOGGING_H_