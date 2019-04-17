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

#ifndef SRC_ALI_SEARCH_ALI_SEARCH_H_
#define SRC_ALI_SEARCH_ALI_SEARCH_H_

#include <string>
#include "ali_search/public_part.h"
#include "ali_search/aoscontext.h"
#include "ali_search/req_task.h"

namespace aos {

class AliOpenSearch : public noncopyable,
  public std::enable_shared_from_this<AliOpenSearch> {
 public:
  typedef std::shared_ptr<AliOpenSearch> Ptr;
  virtual ~AliOpenSearch();

  static AliOpenSearch::Ptr CreateAliOpenSearch(
    AosGlobalContext::Ptr ag_context,
    const std::string &api_domain,
    const std::string &access_key_id,
    const std::string &access_key_secret);

  ////////////////////////////////////////////////////////////////////////////
  // Public Method
  // const char APP_TEMPLATE_BUILTIN_NEWS[] = "builtin_news";
  // const char APP_TEMPLATE_BUILTIN_NOVEL[] = "builtin_novel";
  // const char APP_TEMPLATE_BUILTIN_DOWNLOAD[] = "builtin_download";
  // const char APP_TEMPLATE_BUILTIN_BBS[] = "bbs";
  ResValue::Ptr CreateNewApp(const std::string &app_name,
                             const std::string &template_name);
  //
  ResValue::Ptr GetAppStastus(const std::string &app_name);
  //
  ResValue::Ptr DeleteApp(const std::string &app_name);

  // Push a form to the server
  ResValue::Ptr PushIndexDoc(const std::string &app_name,
                             const std::string &table_name,
                             PushForm::Ptr push_form);

  //
  ResValue::Ptr SuggestHit(const std::string &app_name,
                           const std::string &query,
                           const std::string &suggest_name,
                           int hit_count = 10);

  //
  ResValue::Ptr ReindexApp(const std::string &app_name,
                           const std::string &operate = "",
                           const std::string &table_name = "");

  //
  ResValue::Ptr GetAppErrorLog(const std::string &app_name,
                               int page, int page_size, SortMode sort_mode);

  // Search
  ResValue::Ptr Search(SearchForm::Ptr search_form);

  // Scroll Search
  ResValue::Ptr ScrollSearch(SearchForm::Ptr search_form, Scroll::Ptr scroll);

  ////////////////////////////////////////////////////////////////////////////
  // -------------------------------------------------------------------------
  // About the push
  // Create a push Item
  PushItem::Ptr CreatePushItem(PushItemType type, const std::string &id);
  // Create a push Form
  PushForm::Ptr CreatePushForm(PushItem::Ptr push_item);

  // -------------------------------------------------------------------------
  // About the search
  // Create ConfigStanza
  ConfigStanza::Ptr CreateConfigStanza();

  // Create QueryStanza
  QueryStanza::Ptr CreateQueryStanza(const std::string &index_name,
                                     const std::string &key_word);

  // Create FilterStanza
  FilterStanza::Ptr CreateFilterStanza(const std::string &filter_express);

  // Create SortStanza
  SortStanza::Ptr CreateSortStanza(
    SortType type, const std::string &sort_express);

  // Create AggregateStanza
  AggregateStanza::Ptr CreateAggregateStanza(
    const std::string &group_key, const std::string &agg_fun);

  // Create DistinctStanza
  DistinctStanza::Ptr CreateDistinctStanza(const std::string &dist_key);

  // Create KvpairsStanza
  KvpairsStanza::Ptr CreateKvpairsStanza(
    const std::string &key, const std::string value);

  // Create Query
  Query::Ptr CreateQuery(QueryStanza::Ptr query_stanza);

  // Create Summary
  Summary::Ptr CreateSummary(
    const std::string &summary_field,
    const std::string &summary_element = "em",
    const std::string &summary_ellipsis = "...",
    int summary_snipped = 1);

  // Create SearchFrom
  SearchForm::Ptr CreateSearchForm(
    Query::Ptr query, const std::string &app_name);

  // Create Scroll
  Scroll::Ptr CreateScroll(uint32 scroll_time,
                           ScrollTimeType time_type = SCROLL_TIME_MINUTE,
                           const std::string &search_type = SCROLL_TYPE_SCAN,
                           const std::string &scroll_id = "");
  // -------------------------------------------------------------------------
 private:
  AliOpenSearch(AosGlobalContext::Ptr ag_context,
                const std::string &api_domain,
                const std::string &access_key_id,
                const std::string &access_key_secret);
 private:
  PublicPartManager::Ptr ppm_;
  AosGlobalContext::Ptr ag_context_;
};
}  // namespace aos

#endif  // SRC_ALI_SEARCH_ALI_SEARCH_H_
