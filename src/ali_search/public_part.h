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

#ifndef SRC_ALI_SEARCH_PUBLIC_PART_H_
#define SRC_ALI_SEARCH_PUBLIC_PART_H_

#include <set>
#include "base/baseinclude.h"

namespace aos {
class PublicPartManager : public noncopyable,
  public std::enable_shared_from_this<PublicPartManager> {
 public:
  typedef std::shared_ptr<PublicPartManager> Ptr;
  PublicPartManager(const std::string &api_domain,
                    const std::string &access_key_id,
                    const std::string &access_key_secret);

  // Thread safe
  void CopyPublicPart(std::map<std::string, std::string> &kvs);
  const std::string &access_key_secret() const {
    return access_key_secret_;
  }
  const std::string &api_domain() const {
    return api_domain_;
  }
  virtual ~PublicPartManager();
 public:
  void InitPublicPart(const std::string &access_key_id,
                      const std::string &access_key_secret);
 private:
  std::map<std::string, std::string> keyvalues_;
  std::mutex kvs_mutex_;
  std::string access_key_secret_;
  std::string api_domain_;
};

}  // namespace aos

#endif  // SRC_ALI_SEARCH_PUBLIC_PART_H_


