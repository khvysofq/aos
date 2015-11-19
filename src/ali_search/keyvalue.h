#ifndef ALI_OPENSEARCH_AL_OPENSEARCH_KEY_VALUE_H_
#define ALI_OPENSEARCH_AL_OPENSEARCH_KEY_VALUE_H_

#include "base/baseinclude.h"

namespace aos{

  class KeyValue{
  public:
    explicit KeyValue(const std::string &key, const std::string &value);

    const std::string &key() const{ return key_; }
    const std::string &value() const { return value_; }

    void set_key(const std::string &key){ key_ = key; }
    void set_value(const std::string &value){ value_ = value; }

    bool operator<(const KeyValue &kv) const;
  public:
    std::string key_;
    std::string value_;
  };

}

#endif // ALI_OPENSEARCH_AL_OPENSEARCH_KEY_VALUE_H_