
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

#include <set>
#include <map>

// With basic types
#include "base/basictypes.h"




namespace aos{

  typedef std::map<std::string, std::string > KeyValues;
  typedef std::pair<KeyValues::iterator, bool> KVSInsertResult;
}


#endif // ALI_OPENSEARCH_BASE_BASEINCLUDE_H_