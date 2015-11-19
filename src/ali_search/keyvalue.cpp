#include "ali_search/keyvalue.h"

namespace aos{

  KeyValue::KeyValue(const std::string &key, const std::string &value)
    :key_(key), value_(value){
  }

  bool KeyValue::operator<(const KeyValue &kv) const{
    if (key_ < kv.key()){
      return true;
    }
    if (key_ == kv.key() && value_ < kv.value()){
      return true;
    }
    return false;
  }
}