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

#ifndef SRC_ALI_SEARCH_REQ_TASK_H_
#define SRC_ALI_SEARCH_REQ_TASK_H_

#include <string>
#include "base/baseinclude.h"
#include "ali_search/public_part.h"
#include "ali_search/aoscontext.h"
#include "base/helpmethods.h"
#include "ali_search/res_value.h"
#include "ali_search/req_value.h"

namespace aos {

class BaseReqTask {
 public:
  explicit BaseReqTask(AosGlobalContext::Ptr ag_context,
                       PublicPartManager::Ptr ppmp,
                       const std::string &api_url,
                       HttpMethod http_method);
  virtual ResValue::Ptr SyncStart();
  ResValue::Ptr res_value() {
    return res_value_;
  }
  virtual ~BaseReqTask();

 protected:
  virtual void BuildReqUrl(std::string &result);
  virtual void BuildRequestParames(std::string &parames);
  virtual bool BuildPostData(std::string &post_data) {
    return false;
  }

  virtual bool AddKeyValue(const std::string &key, const std::string &value);
  virtual void AddApiUrl(std::string &result) = 0;
  virtual void AddPrivateKeyValues() = 0;
  virtual ResValue *GetResBuffer() = 0;

  void GeneratorCCQS(std::string &ccqs);
  const std::string GeneratorSignature(const std::string &cqs);

 protected:
  PublicPartManager::Ptr ppmp_;
  AosGlobalContext::Ptr ag_context_;
  KeyValues keyvalues_;
  HttpMethod http_method_;
  std::string api_url_;
  ResValue::Ptr res_value_;
};

//////////////////////////////////////////////////////////////////////////////
class CreateAppTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<CreateAppTask> Ptr;
  virtual ~CreateAppTask();
  ResValue &GetResultValue();
 private:
  explicit CreateAppTask(AosGlobalContext::Ptr ag_context,
                         PublicPartManager::Ptr ppmp,
                         const std::string &app_name,
                         const std::string &template_name);

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }

  friend class AliOpenSearch;
 private:
  std::string app_name_;
  std::string template_name_;
};

//////////////////////////////////////////////////////////////////////////////
class GetAppStatusTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<GetAppStatusTask> Ptr;
  virtual ~GetAppStatusTask();
 private:
  explicit GetAppStatusTask(AosGlobalContext::Ptr ag_context,
                            PublicPartManager::Ptr ppmp,
                            const std::string &app_name);

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }

  friend class AliOpenSearch;
 private:
  std::string app_name_;
};

//////////////////////////////////////////////////////////////////////////////
class DeleteAppTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<DeleteAppTask> Ptr;
  virtual ~DeleteAppTask();
 private:
  explicit DeleteAppTask(AosGlobalContext::Ptr ag_context,
                         PublicPartManager::Ptr ppmp,
                         const std::string &app_name);

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }

  friend class AliOpenSearch;
 private:
  std::string app_name_;
};

//////////////////////////////////////////////////////////////////////////////

class PushIndexDocTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<PushIndexDocTask> Ptr;
  virtual ~PushIndexDocTask();

  PushForm::Ptr push_form() {
    return push_form_;
  }
  void set_push_form(PushForm::Ptr push_form) {
    push_form_ = push_form;
  }

 private:
  explicit PushIndexDocTask(AosGlobalContext::Ptr ag_context,
                            PublicPartManager::Ptr ppmp,
                            const std::string &app_name,
                            const std::string &table_name,
                            PushForm::Ptr push_form);

  virtual bool BuildPostData(std::string &post_data);

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }

  friend class AliOpenSearch;
 private:
  std::string app_name_;
  std::string table_name_;
  PushForm::Ptr push_form_;
};

//////////////////////////////////////////////////////////////////////////////
class SuggestTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<SuggestTask> Ptr;
  virtual ~SuggestTask();
 private:
  explicit SuggestTask(AosGlobalContext::Ptr ag_context,
                       PublicPartManager::Ptr ppmp,
                       const std::string &app_name,
                       const std::string &query,
                       const std::string &suggest_name,
                       int hit_count);

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }

  friend class AliOpenSearch;
 private:
  std::string app_name_;
  std::string suggest_name_;
  std::string query_;
  int hit_count_;
};
//////////////////////////////////////////////////////////////////////////////
class ReindexTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<ReindexTask> Ptr;
  virtual ~ReindexTask();
 private:
  explicit ReindexTask(AosGlobalContext::Ptr ag_context,
                       PublicPartManager::Ptr ppmp,
                       const std::string &app_name,
                       const std::string &operate,
                       const std::string &table_name);

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }
  friend class AliOpenSearch;
 private:
  std::string app_name_;
  std::string operate_;
  std::string table_name_;
};

//////////////////////////////////////////////////////////////////////////////
enum SortMode {
  SORT_ASC,
  SORT_DESC
};
class GetErrorLogTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<GetErrorLogTask> Ptr;
  virtual ~GetErrorLogTask();
 private:
  explicit GetErrorLogTask(AosGlobalContext::Ptr ag_context,
                           PublicPartManager::Ptr ppmp,
                           const std::string &app_name,
                           int page, int page_size, SortMode sort_mode);

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }
  const std::string SortModeToString(SortMode sort_mode);
  friend class AliOpenSearch;
 private:
  std::string app_name_;
  int page_;
  int page_size_;
  SortMode sort_mode_;
};

class SearchTask : public BaseReqTask, public noncopyable {
 public:
  typedef std::shared_ptr<SearchTask> Ptr;
  virtual ~SearchTask();

  virtual ResValue::Ptr SyncStart();

  virtual void AddApiUrl(std::string &result);
  virtual void AddPrivateKeyValues();
  virtual ResValue *GetResBuffer() {
    return res_value_.get();
  }

 private:
  SearchTask(AosGlobalContext::Ptr ag_context,
             PublicPartManager::Ptr ppmp,
             SearchForm::Ptr search_form);

  SearchTask(AosGlobalContext::Ptr ag_context,
             PublicPartManager::Ptr ppmp,
             SearchForm::Ptr search_form,
             Scroll::Ptr scroll);

  friend class AliOpenSearch;
 private:
  SearchForm::Ptr search_form_;
  Scroll::Ptr scroll_;
};
}  // namespace aos

#endif  // SRC_ALI_SEARCH_REQ_TASK_H_
