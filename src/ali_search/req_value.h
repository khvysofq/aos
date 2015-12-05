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

#ifndef SRC_ALI_SEARCH_REQ_VALUE_H_
#define SRC_ALI_SEARCH_REQ_VALUE_H_

#include <set>
#include <vector>
#include <string>
#include <map>
#include "base/baseinclude.h"
#include "base/basedefines.h"
#include "json/json.h"

namespace aos {

class BaseReqValue {
 public:
  virtual const std::string Express() = 0;
  virtual bool IsValue() = 0;
};

enum PushItemType {
  ITEM_TYPE_ADD,
  ITEM_TYPE_UPDATE,
  ITEM_TYPE_DELETE
};

// ---------------------------------------------------------------------------

class PushItem :public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<PushItem> Ptr;
  virtual const std::string Express();
  Json::Value &JsonExpress();
  virtual bool IsValue() {
    return true;
  }

  const std::string &type() const {
    return type_;
  }
  const std::string &id() const {
    return id_;
  }
  const Json::Value &fields() const {
    return fields_json_;
  }

  // JSON
  bool AddField(const std::string &key, const Json::Value &value);
  // TEXT
  bool AddField(const std::string &key, const std::string &value);
  // TEXT ARRAY
  bool AddField(const std::string &key, const std::vector<std::string> &value);
  // INT
  bool AddField(const std::string &key, int value);
  // INT ARRAY
  bool AddField(const std::string &key, const std::vector<int> &value);
  // FLOAT
  bool AddField(const std::string &key, double value);
  // FLOAT ARRAY
  bool AddField(const std::string &key, const std::vector<float> &value);

 private:
  friend class PushIndexDocTask;
  PushItem(PushItemType type, const std::string &id);
  friend class AliOpenSearch;

  const std::string PushItemTypeToString(PushItemType type);

 private:
  std::string type_;
  std::string id_;
  Json::Value item_json_;
  Json::Value fields_json_;
};

class PushForm : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<PushForm> Ptr;
  virtual const std::string Express();
  virtual bool IsValue();

  void AddPushItem(PushItem::Ptr push_item) {
    push_items_.push_back(push_item);
  }
  void ClearPushItems() {
    push_items_.clear();
  }
  void RemovePushItems(PushItem::Ptr push_item);

 private:
  explicit PushForm(PushItem::Ptr push_item);
  friend class AliOpenSearch;
 private:
  std::vector<PushItem::Ptr> push_items_;
};

//////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------
enum TextFormat {
  FORMAT_JSON,
  FORMAT_XML,
  FORMAT_FULLJSON
};

class ConfigStanza : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<ConfigStanza> Ptr;
  virtual const std::string Express();
  virtual bool IsValue();
  //
  uint32 start() {
    return start_;
  }
  void set_start(uint32 start) {
    start_ = start;
  }
  //
  uint32 hit() {
    return hit_;
  }
  void set_hit(uint32 hit) {
    hit_ = hit;
  }
  //
  TextFormat format() {
    return format_;
  }
  void set_format(TextFormat format) {
    format_ = format;
  }
  //
  uint32 rerank_size() {
    return rerank_size_;
  }
  void set_rerank_size(uint32 rerank_size) {
    rerank_size_ = rerank_size;
  }

 private:
  ConfigStanza();
  friend class AliOpenSearch;
  const std::string FormatToString(TextFormat text_format);

 private:
  uint32 start_;
  uint32 hit_;
  TextFormat format_;
  uint32 rerank_size_;
};

// ---------------------------------------------------------------------------
enum QueryUnionType {
  UNION_AND,
  UNION_OR,
  UNION_ANDNOT,
  UNION_RANK
};

class QueryStanza : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<QueryStanza> Ptr;
  virtual const std::string Express() {
    return value_;
  }
  virtual bool IsValue() {
    return !value_.empty();
  }

  QueryStanza::Ptr OrdinaryUnion(QueryUnionType type,
                                 QueryStanza::Ptr query_stanza);
  QueryStanza::Ptr PriorityUnion(QueryUnionType type,
                                 QueryStanza::Ptr query_stanza);

 private:
  QueryStanza(const std::string &key_word,
              const std::string &index_name);
  explicit QueryStanza(const std::string &value);
  const std::string UnionTypeToString(QueryUnionType type);
  friend class AliOpenSearch;

 private:
  std::string value_;
};
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
enum FilterUnionType {
  FILTER_UNION_TYPE_AND,
  FILTER_UNION_TYPE_OR
};

class FilterStanza : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<FilterStanza> Ptr;
  virtual const std::string Express() {
    return filter_express_;
  }
  virtual bool IsValue() {
    return !filter_express_.empty();
  }

  FilterStanza::Ptr OrdinaryUnion(FilterUnionType type,
                                  FilterStanza::Ptr filter_stanza);
  FilterStanza::Ptr PriorityUnion(FilterUnionType type,
                                  FilterStanza::Ptr filter_stanza);

 private:
  explicit FilterStanza(const std::string &filter_express);
  const std::string FilterUnionTypeToString(FilterUnionType type);
  friend class AliOpenSearch;
 private:
  std::string filter_express_;
};

// ---------------------------------------------------------------------------
enum SortType {
  ASC_TYPE,
  DESC_TYPE
};
class SortStanza : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<SortStanza> Ptr;
  virtual const std::string Express() {
    return sort_express_;
  }
  virtual bool IsValue() {
    return !sort_express_.empty();
  }

  SortStanza::Ptr UnionSort(SortStanza::Ptr sort_stanza);

 private:
  explicit SortStanza(const std::string &sort_express);
  SortStanza(SortType type, const std::string &sort_express);
  char SortTypeToChar(SortType type);
  friend class AliOpenSearch;
 private:
  std::string sort_express_;
};

// ---------------------------------------------------------------------------
class AggregateStanza : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<AggregateStanza> Ptr;
  virtual const std::string Express();
  virtual bool IsValue();

  const std::string group_key() {
    return group_key_;
  }

  // about the range
  void AddAggRange(uint32 min, uint32 max);
  void ClearRange();

  //
  void AddAggFunc(const std::string &func_express);
  void ClearAggFunc();

  // filter
  void set_agg_filter(const std::string &agg_filter) {
    agg_filter_ = agg_filter;
  }

  //
  void set_max_group(uint32 max_group) {
    max_group_ = max_group;
  }
  void set_agg_sampler_threshold(uint32 agg_sampler_threshold) {
    agg_sampler_threshold_ = agg_sampler_threshold;
  }
  void set_agg_sampler_step(uint32 agg_sampler_step) {
    agg_sampler_step_ = agg_sampler_step;
  }

 private:
  AggregateStanza(const std::string &group_key, const std::string &agg_fun);
  friend class AliOpenSearch;

 private:
  std::string group_key_;
  std::set<std::string> ranges_;
  std::set<std::string> agg_funcs_;
  std::string agg_filter_;
  uint32 max_group_;
  uint32 agg_sampler_threshold_;
  uint32 agg_sampler_step_;
};
// ---------------------------------------------------------------------------

class DistinctStanza : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<DistinctStanza> Ptr;
  virtual const std::string Express();
  virtual bool IsValue();

  const std::string &dist_key() const {
    return dist_key_;
  }
  //
  void set_dist_times(uint32 dist_times) {
    dist_times_ = dist_times;
  }
  uint32 dist_times() {
    return dist_times_;
  }

  //
  void set_dist_count(uint32 dist_count) {
    dist_count_ = dist_count;
  }
  uint32 dist_count() {
    return dist_count_;
  }

  //
  void set_reserved(bool reserved) {
    reserved_ = reserved;
  }
  bool reserved() {
    return reserved_;
  }

  //
  void set_update_total_hit(bool suth) {
    update_total_hit_ = suth;
  }
  bool update_total_hit() {
    return update_total_hit_;
  }

  //
  void set_dist_filter(const std::string &filter_express) {
    dist_filter_ = filter_express;
  }
  const std::string &dist_filter() {
    return dist_filter_;
  }
  //
  void add_grade(float grade) {
    grades_.push_back(grade);
  }
  void clear_grade() {
    grades_.clear();
  }
  const std::vector<float> &grade() {
    return grades_;
  }

 private:
  explicit DistinctStanza(const std::string &dist_key);
  friend class AliOpenSearch;

 private:
  std::string dist_key_;
  uint32 dist_times_;
  uint32 dist_count_;
  bool reserved_;
  bool update_total_hit_;
  std::string dist_filter_;
  std::vector<float> grades_;
};
// ---------------------------------------------------------------------------

class KvpairsStanza : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<KvpairsStanza> Ptr;
  virtual const std::string Express();
  virtual bool IsValue() {
    return keyvalues_.size() != 0;
  }

  bool AddKvpair(const std::string &key, const std::string value);
  void Clear() {
    keyvalues_.clear();
  }
  std::set<std::string> &keyvalues() {
    return keyvalues_;
  }
 private:
  KvpairsStanza(const std::string &key, const std::string value);
  friend class AliOpenSearch;
 private:
  std::set<std::string> keyvalues_;
};

// ---------------------------------------------------------------------------

class Query : public noncopyable {
 public:
  typedef std::shared_ptr<Query> Ptr;
  virtual const std::string Express();
  virtual bool IsValue();

  // 1. Config
  ConfigStanza::Ptr config_stanza() {
    return config_stanza_;
  }
  void set_config_stanza(ConfigStanza::Ptr config_stanza) {
    config_stanza_ = config_stanza;
  }

  // 2. query
  QueryStanza::Ptr query_stanza() {
    return query_stanza_;
  }
  void set_query_stanza(QueryStanza::Ptr query_stanza) {
    query_stanza_ = query_stanza;
  }

  // 3. filter
  FilterStanza::Ptr filter_stanza() {
    return filter_stanza_;
  }
  void set_filter_stanza(FilterStanza::Ptr filter_stanza) {
    filter_stanza_ = filter_stanza;
  }

  // 4. sort
  SortStanza::Ptr sort_stanza() {
    return sort_stanza_;
  }
  void set_sort_stanza(SortStanza::Ptr sort_stanza) {
    sort_stanza_ = sort_stanza;
  }

  // 5. aggregate
  AggregateStanza::Ptr aggregate_stanza() {
    return aggregate_stanza_;
  }
  void set_aggregate_stanza(AggregateStanza::Ptr aggregate_stanza) {
    aggregate_stanza_ = aggregate_stanza;
  }

  // 6. distinct
  DistinctStanza::Ptr distinct_stanza() {
    return distinct_stanza_;
  }
  void set_distinct_stanza(DistinctStanza::Ptr distinct_stanza) {
    distinct_stanza_ = distinct_stanza;
  }

  // 7. kvpairs
  KvpairsStanza::Ptr kvpair_stanza() {
    return kvpair_stanza_;
  }
  void set_kvpair_stanza(KvpairsStanza::Ptr kvpair_stanza) {
    kvpair_stanza_ = kvpair_stanza;
  }

 private:
  explicit Query(QueryStanza::Ptr query_stanza);
  void AddKeyExpress(std::string &express, const std::string &key,  // NOLINT
                     const std::string &value);
  friend class AliOpenSearch;
 private:
  ConfigStanza::Ptr config_stanza_;
  QueryStanza::Ptr query_stanza_;
  FilterStanza::Ptr filter_stanza_;
  SortStanza::Ptr sort_stanza_;
  AggregateStanza::Ptr aggregate_stanza_;
  DistinctStanza::Ptr distinct_stanza_;
  KvpairsStanza::Ptr kvpair_stanza_;
};

class Summary : public noncopyable, public BaseReqValue {
 public:
  typedef std::shared_ptr<Summary> Ptr;
  virtual const std::string Express();
  virtual bool IsValue();

  bool ToSummaryString(std::string &summary);  // NOLINT

  bool IsInvalueSummary() {
    return summary_field_.empty();
  }

  // summary_field
  const std::string &summary_field() const {
    return summary_field_;
  }
  bool set_summary_filed(const std::string &summary_filed);

  // summary_element
  const std::string &summary_element() const {
    return summary_element_;
  }
  bool set_summary_element(const std::string &summary_element);

  // summary ellipsis
  const std::string &summary_ellipsis() const {
    return summary_ellipsis_;
  }
  bool set_summary_ellipsis(const std::string &summary_ellipsis);

  // summary snipped
  uint32 summary_snipped() const {
    return summary_snipped_;
  }
  bool set_summary_snipped(uint32 summary_snipped);

  // summary len
  uint32 summary_len() const {
    return summary_len_;
  }
  bool set_summary_len(uint32 summary_len);

  // summary prefix and summary postfix
  const std::string &summary_prefix() const {
    return summary_prefix_;
  }
  const std::string &summary_postfix() const {
    return summary_postfix_;
  }
  bool set_summary_prefix_postfix(
    const std::string &summary_prefix, const std::string &summary_postfix);

 private:
  Summary(const std::string &summary_field,
          const std::string &summary_element,
          const std::string &summary_ellipsis,
          int summary_snipped);

  void AddKeyValueToString(std::string &str,  // NOLINT
                           const std::string &key, const std::string &value);  // NOLINT
  friend class AliOpenSearch;
 private:
  std::string summary_field_;
  std::string summary_element_;
  std::string summary_ellipsis_;
  uint32 summary_snipped_;
  uint32 summary_len_;
  std::string summary_prefix_;
  std::string summary_postfix_;
};

class SearchForm : public noncopyable {
 public:
  typedef std::shared_ptr<SearchForm> Ptr;
  // virtual const std::string Express();
  bool IsValue();

  // About Query
  Query::Ptr query() {
    return query_;
  }
  void set_query(Query::Ptr query) {
    query_ = query;
  }
  void ClearQuery() {
    query_.reset();
  }

  // About index name
  const std::set<std::string> &index_app_names() {
    return index_app_names_;
  }
  void AddSearchAppName(const std::string &app_name);
  void RemoveSearchAppName(const std::string &app_name);
  void ClearSearchAppName() {
    index_app_names_.clear();
  }

  // Fetch fileds
  const std::set<std::string> &fetch_fields() {
    return fetch_fields_;
  }
  void AddFetchField(const std::string &fetch_field);
  void RemoveFetchField(const std::string &fetch_field);
  void ClearFetchField() {
    fetch_fields_.clear();
  }

  // About qp
  const std::set<std::string> &query_protos() {
    return query_protos_;
  }
  void AddQueryProto(const std::string &proto);
  void RemoveQueryProto(const std::string &proto);
  void ClearQueryProto() {
    query_protos_.clear();
  }

  // About disable, No disable settings

  // About first_formula_name
  const std::string &first_formula_name() {
    return first_formula_name_;
  }
  void set_first_formula_name(const std::string &first_formula_name) {
    first_formula_name_ = first_formula_name;
  }
  void ClearFisrtFormulaName() {
    first_formula_name_.clear();
  }

  // About formula name
  const std::string &formula_name() {
    return formula_name_;
  }
  void set_formula_name(const std::string &formula_name) {
    formula_name_ = formula_name;
  }
  void ClearFormulaName() {
    formula_name_.clear();
  }

  // About summary
  Summary::Ptr summary() {
    return summary_;
  }
  void ClearSummary() {
    summary_.reset();
  }
  void set_summary(Summary::Ptr summary) {
    summary_ = summary;
  }

 private:
  SearchForm(Query::Ptr query, const std::string &app_name);
  void AddExpress(std::string &express,  // NOLINT
                  const std::string &key, const std::string value);  // NOLINT
  const std::string SearchAppNameExpress();
  const std::string FetchFieldsExpress();
  const std::string QueryProtoExpress();

  bool AddKeyValue(std::map<std::string, std::string> &kvs,  // NOLINT
                   const std::string &key, const std::string &value);  // NOLINT
  bool UpdateKeyValue(std::map<std::string, std::string> &kvs);  // NOLINT
  friend class SearchTask;
  friend class AliOpenSearch;
 private:
  Query::Ptr query_;
  std::set<std::string> index_app_names_;
  std::set<std::string> fetch_fields_;
  std::set<std::string> query_protos_;
  std::string disable_;
  std::string first_formula_name_;
  std::string formula_name_;
  Summary::Ptr summary_;
  bool enable_summary_;
};

//////////////////////////////////////////////////////////////////////////////
//
enum ScrollTimeType {
  SCROLL_TIME_SECOND,
  SCROLL_TIME_MINUTE,
  SCROLL_TIME_HOUR,
  SCROLL_TIME_DAY,
  SCROLL_TIME_WEEK
};

class Scroll : public noncopyable {
 public:
  typedef std::shared_ptr<Scroll> Ptr;

  // scroll time
  uint32 scroll_time() {
    return scroll_time_;
  }
  ScrollTimeType time_type() {
    return time_type_;
  }
  void set_scroll_time(uint32 scroll_time, ScrollTimeType time_type) {
    scroll_time_ = scroll_time;
    time_type_ = time_type;
  }

  // Search type
  const std::string &search_type() {
    return search_type_;
  }
  void set_search_type(const std::string &search_type) {
    search_type_ = search_type;
  }

  // Scroll id
  const std::string &scroll_id() {
    return scroll_id_;
  }
  void set_scroll_id(const std::string &scroll_id) {
    scroll_id_ = scroll_id;
  }

 private:
  Scroll(uint32 scroll_time, ScrollTimeType time_type = SCROLL_TIME_MINUTE,
         const std::string &search_type = SCROLL_TYPE_SCAN,
         const std::string &scroll_id = "");

  void HandleResultMessage(Json::Value &result_json);  // NOLINT
  bool UpdateKeyValue(std::map<std::string, std::string> &kvs);  // NOLINT
  const std::string GetScrollTime();
  char ScrollTimeTypeToString(ScrollTimeType time_type);

  friend class AliOpenSearch;
  friend class SearchTask;
 private:
  uint32 scroll_time_;
  ScrollTimeType time_type_;
  std::string search_type_;
  std::string scroll_id_;
};

}  // namespace aos

#endif  // SRC_ALI_SEARCH_REQ_VALUE_H_
