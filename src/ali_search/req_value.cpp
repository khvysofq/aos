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

#include "ali_search/req_value.h"
#include <map>
#include <string>
#include <vector>
#include <set>
#include "base/basedefines.h"
#include "base/common.h"
#include "base/helpmethods.h"
#include "base/logging.h"
#include "json/json.h"


namespace aos {

// ---------------------------------------------------------------------------
PushItem::PushItem(PushItemType type, const std::string &id) :id_(id) {
  item_json_[JSON_PUSH_CMD] = PushItemTypeToString(type);
  item_json_[JSON_PUSH_TIMESTAMP] = HelpMethos::GetUnixTimeStamp();
  fields_json_[JSON_PUSH_ITEM_ID] = id;
}

Json::Value &PushItem::JsonExpress() {
  item_json_[JSON_PUSH_FIELDS] = fields_json_;
  return item_json_;
}

const std::string PushItem::Express() {
  return JsonExpress().toStyledString();
}

bool PushItem::AddField(const std::string &key, const Json::Value &value) {
  try {
    fields_json_[key] = value;
  } catch (std::exception &e) {
    LOG_ERROR << e.what();
    return false;
  }
  return true;
}

// JSON
bool PushItem::AddField(const std::string &key, const std::string &value) {
  try {
    fields_json_[key] = value;
  } catch (std::exception &e) {
    LOG_ERROR << e.what();
    return false;
  }
  return true;
}

// TEXT ARRAY
bool PushItem::AddField(const std::string &key,
                        const std::vector<std::string> &value) {
  try {
    Json::Value json_array;
    for (std::size_t i = 0; i < value.size(); i++) {
      json_array.append(value[i]);
    }
    fields_json_[key] = json_array;
  } catch (std::exception &e) {
    LOG_ERROR << e.what();
    return false;
  }
  return true;
}

// INT
bool PushItem::AddField(const std::string &key, int value) {
  try {
    fields_json_[key] = value;
  } catch (std::exception &e) {
    LOG_ERROR << e.what();
    return false;
  }
  return true;
}

// INT ARRAY
bool PushItem::AddField(const std::string &key, const std::vector<int> &value) {
  try {
    Json::Value json_array;
    for (std::size_t i = 0; i < value.size(); i++) {
      json_array.append(value[i]);
    }
    fields_json_[key] = json_array;
  } catch (std::exception &e) {
    LOG_ERROR << e.what();
    return false;
  }
  return true;
}

// FLOAT
bool PushItem::AddField(const std::string &key, double value) {
  try {
    fields_json_[key] = value;
  } catch (std::exception &e) {
    LOG_ERROR << e.what();
    return false;
  }
  return true;
}

// FLOAT ARRAY
bool PushItem::AddField(const std::string &key, std::vector<float> &value) {
  try {
    Json::Value json_array;
    for (std::size_t i = 0; i < value.size(); i++) {
      json_array.append(value[i]);
    }
    fields_json_[key] = json_array;
  } catch (std::exception &e) {
    LOG_ERROR << e.what();
    return false;
  }
  return true;
}
const std::string PushItem::PushItemTypeToString(PushItemType type) {
  switch (type) {
  case aos::ITEM_TYPE_ADD:
    return "add";
    break;
  case aos::ITEM_TYPE_UPDATE:
    return "update";
    break;
  case aos::ITEM_TYPE_DELETE:
    return "delete";
    break;
  default:
    break;
  }
  ASSERT(false);
  return "update";
}
// ---------------------------------------------------------------------------

PushForm::PushForm(PushItem::Ptr push_item) {
  AddPushItem(push_item);
}

bool PushForm::IsValue() {
  if (push_items_.size() > 0) {
    return true;
  }
  return false;
}

const std::string PushForm::Express() {
  Json::Value express(Json::arrayValue);
  Json::FastWriter fw;
  if (!IsValue()) {
    return fw.write(express);
  }
  for (std::size_t i = 0; i < push_items_.size(); i++) {
    if (push_items_[i]->IsValue()) {
      express.append(push_items_[i]->JsonExpress());
    }
  }
  return fw.write(express);
}

void PushForm::RemovePushItems(PushItem::Ptr push_item) {
  for (std::size_t i = 0; i < push_items_.size(); i++) {
    if (push_items_[i] == push_item) {
      push_items_.erase(push_items_.begin() + i);
      return;
    }
  }
}

// ---------------------------------------------------------------------------
ConfigStanza::ConfigStanza()
  :start_(0), hit_(10), format_(FORMAT_JSON), rerank_size_(200) {
}

bool ConfigStanza::IsValue() {
  if (start_ == 0
      && hit_ == 10
      && format_ == FORMAT_JSON
      && rerank_size_ == 200) {
    return false;
  }
  return true;
}

const std::string ConfigStanza::Express() {
  std::string express;
  if (!IsValue()) {
    return express;
  }
  if (start_ != 0) {
    express.append(QUERY_CONFIG_START);
    express.push_back(':');
    express.append(std::to_string(start_));
  }
  if (hit_ != 10) {
    express.append(QUERY_CONFIG_HIT);
    express.push_back(':');
    express.append(std::to_string(hit_));
  }
  if (format_ != FORMAT_JSON) {
    express.append(QUERY_CONFIG_FORMAT);
    express.push_back(':');
    express.append(FormatToString(format_));
  }
  if (rerank_size_ != 200) {
    express.append(QUERY_CONFIG_RERANK_SIZE);
    express.push_back(':');
    express.append(std::to_string(rerank_size_));
  }
  return express;
}

const std::string ConfigStanza::FormatToString(TextFormat text_format) {
  switch (text_format) {
  case aos::FORMAT_JSON:
    return QUERY_CONFIG_FORMAT_JSON;
    break;
  case aos::FORMAT_XML:
    return QUERY_CONFIG_FORMAT_XML;
    break;
  case aos::FORMAT_FULLJSON:
    return QUERY_CONFIG_FORMAT_FULLJSON;
    break;
  default:
    break;
  }
  ASSERT(false);
  return QUERY_CONFIG_FORMAT_JSON;
}

//----------------------------------------------------------------------------
QueryStanza::QueryStanza(const std::string &key_word,
                         const std::string &index_name) {
  value_.append(index_name);
  value_.push_back(':');
  value_.push_back('\'');
  value_.append(key_word);
  value_.push_back('\'');
}

QueryStanza::QueryStanza(const std::string &value) :value_(value) {
}

QueryStanza::Ptr QueryStanza::OrdinaryUnion(QueryUnionType type,
    QueryStanza::Ptr query_stanza) {
  ASSERT(query_stanza.get() != NULL);
  std::string new_value;
  new_value.append(value_);
  new_value.push_back(' ');
  new_value.append(UnionTypeToString(type));
  new_value.push_back(' ');
  new_value.append(query_stanza->Express());
  return QueryStanza::Ptr(new QueryStanza(new_value));
}

QueryStanza::Ptr QueryStanza::PriorityUnion(QueryUnionType type,
    QueryStanza::Ptr query_stanza) {
  ASSERT(query_stanza.get() != NULL);
  std::string new_value;
  new_value.push_back('(');
  new_value.append(value_);
  new_value.push_back(')');
  new_value.push_back(' ');
  new_value.append(UnionTypeToString(type));
  new_value.push_back(' ');
  new_value.append(query_stanza->Express());
  return QueryStanza::Ptr(new QueryStanza(new_value));
}

const std::string QueryStanza::UnionTypeToString(QueryUnionType type) {
  switch (type) {
  case aos::UNION_AND:
    return UNION_TYPE_AND;
    break;
  case aos::UNION_OR:
    return UNION_TYPE_OR;
    break;
  case aos::UNION_ANDNOT:
    return UNION_TYPE_ANDNOT;
    break;
  case aos::UNION_RANK:
    return UNION_TYPE_RANK;
    break;
  default:
    break;
  }
  ASSERT(false);
  return UNION_TYPE_AND;
}
//----------------------------------------------------------------------------

FilterStanza::FilterStanza(const std::string &filter_express)
  :filter_express_(filter_express) {
}

FilterStanza::Ptr FilterStanza::OrdinaryUnion(FilterUnionType type,
    FilterStanza::Ptr filter_stanza) {
  std::string new_filter_stanza;
  new_filter_stanza.append(filter_express_);
  new_filter_stanza.push_back(' ');
  new_filter_stanza.append(FilterUnionTypeToString(type));
  new_filter_stanza.push_back(' ');
  new_filter_stanza.append(filter_stanza->Express());
  return FilterStanza::Ptr(new FilterStanza(new_filter_stanza));
}

FilterStanza::Ptr FilterStanza::PriorityUnion(FilterUnionType type,
    FilterStanza::Ptr filter_stanza) {
  std::string new_filter_stanza;
  new_filter_stanza.push_back('(');
  new_filter_stanza.append(filter_express_);
  new_filter_stanza.push_back(')');
  new_filter_stanza.push_back(' ');
  new_filter_stanza.append(FilterUnionTypeToString(type));
  new_filter_stanza.push_back(' ');
  new_filter_stanza.append(filter_stanza->Express());
  return FilterStanza::Ptr(new FilterStanza(new_filter_stanza));
}

const std::string FilterStanza::FilterUnionTypeToString(FilterUnionType type) {
  switch (type) {
  case aos::FILTER_UNION_TYPE_AND:
    return UNION_TYPE_AND;
    break;
  case aos::FILTER_UNION_TYPE_OR:
    return UNION_TYPE_OR;
    break;
  default:
    break;
  }
  ASSERT(false);
  return UNION_TYPE_AND;
}
//----------------------------------------------------------------------------
SortStanza::SortStanza(const std::string &sort_express)
  :sort_express_(sort_express) {
}

SortStanza::SortStanza(SortType type, const std::string &sort_express) {
  sort_express_.push_back(SortTypeToChar(type));
  sort_express_.append(sort_express);
}

SortStanza::Ptr SortStanza::UnionSort(SortStanza::Ptr sort_stanza) {
  std::string new_sort_stanza;
  new_sort_stanza.append(sort_express_);
  new_sort_stanza.push_back(';');
  new_sort_stanza.append(sort_stanza->Express());
  return SortStanza::Ptr(new SortStanza(new_sort_stanza));
}

char SortStanza::SortTypeToChar(SortType type) {
  switch (type) {
  case aos::ASC_TYPE:
    return '+';
    break;
  case aos::DESC_TYPE:
    return '-';
    break;
  default:
    break;
  }
  return '+';
}


//----------------------------------------------------------------------------
AggregateStanza::AggregateStanza(const std::string &group_key,
                                 const std::string &agg_fun):
  group_key_(group_key),
  max_group_(0),
  agg_sampler_threshold_(0),
  agg_sampler_step_(0) {
  ASSERT(!group_key.empty());
  agg_funcs_.insert(agg_fun);
}

bool AggregateStanza::IsValue() {
  if (group_key_.empty() || agg_funcs_.size() == 0) {
    return false;
  }
  return true;
}

const std::string AggregateStanza::Express() {
  std::string express;
  if (!IsValue()) {
    return express;
  }
  // 1. group key
  express.append(QUERY_AGGREGATE_GROUP_KEY);
  express.push_back(':');
  express.append(group_key_);
  // 2. range
  for (std::set<std::string>::iterator iter = ranges_.begin();
       iter != ranges_.end(); ++iter) {
    express.push_back(',');
    express.append(QUERY_AGGREGATE_RANGE);
    express.push_back(':');
    express.append(*iter);
  }
  // 3. agg_func
  if (agg_funcs_.size() > 0) {
    express.push_back(',');
    express.append(QUERY_AGGREGATE_FUN);
    express.push_back(':');
    std::set<std::string>::iterator iter = agg_funcs_.begin();
    while (iter != agg_funcs_.end()) {
      express.append(*iter);
      ++iter;
      if (iter != agg_funcs_.end()) {
        express.push_back('#');
      }
    }
  }
  // 4. max_group
  if (max_group_) {
    express.push_back(',');
    express.append(QUERY_AGGREGATE_MAX_GROUP);
    express.push_back(':');
    express.append(std::to_string(max_group_));
  }
  // 5. agg_filter
  if (!agg_filter_.empty()) {
    express.push_back(',');
    express.append(QUERY_AGGREGATE_FILTER);
    express.push_back(':');
    express.append(agg_filter_);
  }
  // 6. agg_sampler_threshold
  if (agg_sampler_threshold_) {
    express.push_back(',');
    express.append(QUERY_AGGREGATE_AST);
    express.push_back(':');
    express.append(std::to_string(agg_sampler_threshold_));
  }
  // 7. agg_sampler_step
  if (agg_sampler_step_) {
    express.push_back(',');
    express.append(QUERY_AGGREGATE_ASS);
    express.push_back(':');
    express.append(std::to_string(agg_sampler_step_));
  }
  return express;
}

void AggregateStanza::AddAggRange(uint32 min, uint32 max) {
  std::string range_str;
  range_str.append(QUERY_AGGREGATE_RANGE);
  range_str.push_back(':');
  range_str.append(std::to_string(min));
  range_str.push_back('~');
  range_str.append(std::to_string(max));
  ranges_.insert(range_str);
}

void AggregateStanza::ClearRange() {
  ranges_.clear();
}

void AggregateStanza::AddAggFunc(const std::string &func_express) {
  agg_funcs_.insert(func_express);
}

void AggregateStanza::ClearAggFunc() {
  agg_funcs_.clear();
}
//----------------------------------------------------------------------------
DistinctStanza::DistinctStanza(const std::string &dist_key):
  dist_key_(dist_key),
  dist_times_(1),
  dist_count_(1),
  reserved_(true),
  update_total_hit_(false) {
  ASSERT(!dist_key.empty());
}

bool DistinctStanza::IsValue() {
  if (dist_key_.empty()) {
    return false;
  }
  return true;
}

const std::string DistinctStanza::Express() {
  std::string distinct_express;
  // 1. add dist_key
  if (!IsValue()) {
    return distinct_express;
  } else {
    distinct_express.append(QUERY_DIST_KEY);
    distinct_express.push_back(':');
    distinct_express.append(dist_key_);
  }
  // 2
  if (dist_times_ != 1) {
    distinct_express.push_back(',');
    distinct_express.append(QUERY_DIST_TIMES);
    distinct_express.push_back(':');
    distinct_express.append(std::to_string(dist_times_));
  }
  // 3.
  if (dist_count_ != 1) {
    distinct_express.push_back(',');
    distinct_express.append(QUERY_DIST_COUNT);
    distinct_express.push_back(':');
    distinct_express.append(std::to_string(dist_count_));
  }
  // 4.
  if (!reserved_) {
    distinct_express.push_back(',');
    distinct_express.append(QUERY_DIST_RESERVED);
    distinct_express.push_back(':');
    distinct_express.append(std::to_string(reserved_));
  }
  // 5.
  if (update_total_hit_) {
    distinct_express.push_back(',');
    distinct_express.append(QUERY_DIST_UTH);
    distinct_express.push_back(':');
    distinct_express.append(std::to_string(update_total_hit_));
  }
  // 6. filter
  if (!dist_filter_.empty()) {
    distinct_express.push_back(',');
    distinct_express.append(QUERY_DIST_DIST_FILTER);
    distinct_express.push_back(':');
    distinct_express.append(dist_filter_);
  }
  // 6.grades
  if (grades_.size() > 0) {
    distinct_express.push_back(',');
    distinct_express.append(QUERY_DIST_GRADE);
    distinct_express.push_back(':');
    std::size_t i = 0;
    while (i < grades_.size()) {
      distinct_express.append(std::to_string(grades_[i]));
      i++;
      if (i < grades_.size()) {
        distinct_express.push_back('|');
      }
    }
  }
  return distinct_express;
}

//----------------------------------------------------------------------------
KvpairsStanza::KvpairsStanza(const std::string &key, const std::string value) {
  AddKvpair(key, value);
}

bool KvpairsStanza::AddKvpair(
  const std::string &key, const std::string value) {
  if (key.empty() || value.empty()) {
    return false;
  }
  std::string kvpair;
  kvpair.append(key);
  kvpair.push_back(':');
  kvpair.append(value);
  keyvalues_.insert(kvpair);
  return true;
}

const std::string KvpairsStanza::Express() {
  std::string kvpairs_express;
  std::set<std::string>::iterator iter = keyvalues_.begin();
  while (iter != keyvalues_.end()) {
    kvpairs_express.append(*iter);
    ++iter;
    if (iter != keyvalues_.end()) {
      kvpairs_express.push_back(',');
    }
  }
  return kvpairs_express;
}
//----------------------------------------------------------------------------

Query::Query(QueryStanza::Ptr query_stanza)
  :query_stanza_(query_stanza) {
}

bool Query::IsValue() {
  if (query_stanza_ && query_stanza_->IsValue()) {
    return true;
  }
  return false;
}

const std::string Query::Express() {
  std::string express;
  if (!IsValue()) {
    return express;
  }
  // 1. query
  AddKeyExpress(express, QUERY_QUERY_STANZA, query_stanza_->Express());

  // 2. config
  if (config_stanza_ && config_stanza_->IsValue()) {
    express.append("&&");
    AddKeyExpress(express, QUERY_CONFIG_STANZA, config_stanza_->Express());
  }

  // 3. filter
  if (filter_stanza_ && filter_stanza_->IsValue()) {
    express.append("&&");
    AddKeyExpress(express, QUERY_FILTER_STANZA, filter_stanza_->Express());
  }

  // 4. sort
  if (sort_stanza_ && sort_stanza_->IsValue()) {
    express.append("&&");
    AddKeyExpress(express, QUERY_SORT_STANZA, sort_stanza_->Express());
  }

  // 5. aggregate
  if (aggregate_stanza_ && aggregate_stanza_->IsValue()) {
    express.append("&&");
    AddKeyExpress(express, QUERY_AGGREGATE_STANZA,
                  aggregate_stanza_->Express());
  }

  // 6. distinct
  if (distinct_stanza_ && distinct_stanza_->IsValue()) {
    express.append("&&");
    AddKeyExpress(express, QUERY_DISTINCT_STANZA,
                  distinct_stanza_->Express());
  }

  // 7. kvpairs
  if (kvpair_stanza_ && kvpair_stanza_->IsValue()) {
    express.append("&&");
    AddKeyExpress(express, QUERY_KVPAIRS_STANZA, kvpair_stanza_->Express());
  }
  return express;
}

void Query::AddKeyExpress(std::string &express, const std::string &key,
                          const std::string &value) {
  express.append(key);
  express.push_back('=');
  express.append(value);
}

//----------------------------------------------------------------------------
Summary::Summary(const std::string &summary_field,
                 const std::string &summary_element,
                 const std::string &summary_ellipsis,
                 int summary_snipped):
  summary_field_(summary_field),
  summary_element_(summary_element),
  summary_ellipsis_(summary_ellipsis),
  summary_snipped_(summary_snipped),
  summary_len_(0) {
}

bool Summary::IsValue() {
  if (summary_field_.empty()) {
    return false;
  }
  return true;
}

const std::string Summary::Express() {
  std::string summary;
  if (!IsValue()) {
    return summary;
  }
  // 1. add the filed
  AddKeyValueToString(summary, RES_SUMMARY_FIELD, summary_field_);
  // 2. add element
  if (!summary_element_.empty()) {
    summary.push_back(',');
    AddKeyValueToString(summary, RES_SUMMARY_ELEMENT, summary_element_);
  }
  // 3. add ellipsis
  if (!summary_ellipsis_.empty()) {
    summary.push_back(',');
    AddKeyValueToString(summary, RES_SUMMARY_ELLIPSIS, summary_ellipsis_);
  }
  // 4. add snipped
  if (summary_snipped_ >= 1) {
    summary.push_back(',');
    AddKeyValueToString(summary, RES_SUMMARY_SNIPPED,
                        std::to_string(summary_snipped_));
  }
  // 5. add len
  if (summary_len_ > 0) {
    summary.push_back(',');
    AddKeyValueToString(summary, RES_SUMMARY_LEN,
                        std::to_string(summary_len_));
  }
  // 6. add summary prefix and summary postfix
  if (!summary_prefix_.empty() && !summary_postfix_.empty()) {
    summary.push_back(',');
    AddKeyValueToString(summary, RES_SUMMARY_PREFIX, summary_prefix_);
    summary.push_back(',');
    AddKeyValueToString(summary, RES_SUMMARY_POSTFIX, summary_postfix_);
  }
  return summary;
}

void Summary::AddKeyValueToString(std::string &str,
                                  const std::string &key,
                                  const std::string &value) {
  str.append(key);
  str.push_back(':');
  str.append(value);
}

bool Summary::set_summary_filed(const std::string &summary_filed) {
  if (summary_filed.empty()) {
    return false;
  }
  summary_field_ = summary_filed;
  return true;
}

bool Summary::set_summary_element(const std::string &summary_element) {
  if (summary_element.empty()) {
    return false;
  }
  summary_element_ = summary_element;
  // clear the summary_prefix and summary postfix
  summary_prefix_.clear();
  summary_postfix_.clear();

  return true;
}

bool Summary::set_summary_ellipsis(const std::string &summary_ellipsis) {
  if (summary_ellipsis.empty()) {
    return false;
  }
  summary_ellipsis_ = summary_ellipsis;
  return true;
}

bool Summary::set_summary_snipped(uint32 summary_snipped) {
  if (summary_snipped < 1) {
    return false;
  }
  summary_snipped_ = summary_snipped;
  return true;
}

bool Summary::set_summary_len(uint32 summary_len) {
  summary_len_ = summary_len;
  return true;
}

bool Summary::set_summary_prefix_postfix(
  const std::string &summary_prefix, const std::string &summary_postfix) {
  if (summary_prefix.empty() || summary_postfix_.empty()) {
    return false;
  }
  // set the summary_element with null
  summary_element_.clear();
  // Set the summary fix value
  summary_prefix_ = summary_prefix;
  summary_postfix_ = summary_postfix;
  return true;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
SearchForm::SearchForm(Query::Ptr query, const std::string &app_name):
  enable_summary_(false),
  query_(query) {
  AddSearchAppName(app_name);
}

bool SearchForm::IsValue() {
  if (query_ && query_->IsValue() && index_app_names_.size() > 0) {
    return true;
  }
  return false;
}

// const std::string SearchForm::Express(){
//  std::string express;
//  // Start setting the key values to null
//  key_values_.clear();

//  if (!IsValue()){
//    return express;
//  }
//  // 1. Add query
//  AddExpress(express, RES_SEARCH_QUERY, query_->Express());
//  // 2. Add index name
//  express.append("&");
//  AddExpress(express, RES_SEARCH_INDEX_NAME, SearchAppNameExpress());
//  // 3. Add fetch fields
//  if (fetch_fields_.size() != 0){
//    express.append("&");
//    AddExpress(express, RES_SEARCH_FETCH_FIELDS, FetchFieldsExpress());
//  }
//  // 4. Add qp
//  if (query_protos_.size() != 0){
//    express.append("&");
//    AddExpress(express, RES_SEAERCH_QP, QueryProtoExpress());
//  }
//  // 5. Add disable
//  // The document is not clear
//  // 6. Add first_formula_name
//  if (!first_formula_name_.empty()){
//    express.append("&");
//    AddExpress(express, RES_SEARCH_FFN, first_formula_name_);
//  }
//  // 7. Add formula_name
//  if (!formula_name_.empty()){
//    express.append("&");
//    AddExpress(express, RES_SEARCH_FN, formula_name_);
//  }
//  // 8. Add summary
//  if (summary_ && summary_->IsValue()){
//    express.append("&");
//    AddExpress(express, RES_SEARCH_SUMMARY, summary_->Express());
//  }
//  return express;
//}

// void SearchForm::AddExpress(std::string &express,
//  const std::string &key, const std::string value){
//  express.append(key);
//  express.push_back('=');
//  express.append(HelpMethos::URLEncode(value));
// }

bool SearchForm::AddKeyValue(std::map<std::string, std::string> &kvs,
                             const std::string &key, const std::string &value) {
  KVSInsertResult res = kvs.insert(std::make_pair(key, value));
  if (res.second == false) {
    LOG_ERROR << "Add key value error, maybe the same key aready exsits!";
  }
  return res.second;
}

bool SearchForm::UpdateKeyValue(std::map<std::string, std::string> &kvs) {
  if (!IsValue()) {
    return false;
  }
  // 1. Add query
  AddKeyValue(kvs, RES_SEARCH_QUERY, query_->Express());
  // 2. Add index name
  AddKeyValue(kvs, RES_SEARCH_INDEX_NAME, SearchAppNameExpress());
  // 3. Add fetch fields
  if (fetch_fields_.size() != 0) {
    AddKeyValue(kvs, RES_SEARCH_FETCH_FIELDS, FetchFieldsExpress());
  }
  // 4. Add qp
  if (query_protos_.size() != 0) {
    AddKeyValue(kvs, RES_SEAERCH_QP, QueryProtoExpress());
  }
  // 5. Add disable
  // The document is not clear
  // 6. Add first_formula_name
  if (!first_formula_name_.empty()) {
    AddKeyValue(kvs, RES_SEARCH_FFN, first_formula_name_);
  }
  // 7. Add formula_name
  if (!formula_name_.empty()) {
    AddKeyValue(kvs, RES_SEARCH_FN, formula_name_);
  }
  // 8. Add summary
  if (summary_ && summary_->IsValue()) {
    AddKeyValue(kvs, RES_SEARCH_SUMMARY, summary_->Express());
  }
  return true;
}

// Search app name
void SearchForm::AddSearchAppName(const std::string &app_name) {
  index_app_names_.insert(app_name);
}

void SearchForm::RemoveSearchAppName(const std::string &app_name) {
  if (index_app_names_.size() == 0) {
    return;
  }
  std::set<std::string>::iterator iter = index_app_names_.find(app_name);
  if (iter != index_app_names_.end()) {
    index_app_names_.erase(iter);
  }
}

const std::string SearchForm::SearchAppNameExpress() {
  std::string express;
  std::set<std::string>::iterator iter = index_app_names_.begin();
  while (iter != index_app_names_.end()) {
    express.append(*iter);
    ++iter;
    if (iter != index_app_names_.end()) {
      express.push_back(';');
    }
  }
  return express;
}

// fetch field
void SearchForm::AddFetchField(const std::string &fetch_field) {
  fetch_fields_.insert(fetch_field);
}

void SearchForm::RemoveFetchField(const std::string &fetch_field) {
  if (fetch_fields_.size() == 0) {
    return;
  }
  std::set<std::string>::iterator iter = fetch_fields_.find(fetch_field);
  if (iter != fetch_fields_.end()) {
    fetch_fields_.erase(iter);
  }
}

const std::string SearchForm::FetchFieldsExpress() {
  std::string express;
  std::set<std::string>::iterator iter = fetch_fields_.begin();
  while (iter != fetch_fields_.end()) {
    express.append(*iter);
    ++iter;
    if (iter != fetch_fields_.end()) {
      express.push_back(';');
    }
  }
  return express;
}

// query proto
void SearchForm::AddQueryProto(const std::string &proto) {
  query_protos_.insert(proto);
}

void SearchForm::RemoveQueryProto(const std::string &proto) {
  if (query_protos_.size() == 0) {
    return;
  }
  std::set<std::string>::iterator iter = query_protos_.find(proto);
  if (iter != query_protos_.end()) {
    query_protos_.erase(iter);
  }
}

const std::string SearchForm::QueryProtoExpress() {
  std::string express;
  std::set<std::string>::iterator iter = query_protos_.begin();
  while (iter != query_protos_.end()) {
    express.append(*iter);
    ++iter;
    if (iter != query_protos_.end()) {
      express.push_back(',');
    }
  }
  return express;
}

//////////////////////////////////////////////////////////////////////////////
//
Scroll::Scroll(uint32 scroll_time,
               ScrollTimeType time_type,
               const std::string &search_type,
               const std::string &scroll_id):
  scroll_time_(scroll_time),
  time_type_(time_type),
  search_type_(search_type),
  scroll_id_(scroll_id) {
}

void Scroll::HandleResultMessage(Json::Value &result_json) {
  if (!result_json.isNull() && !result_json[RES_SCROLL_ID].isNull()) {
    scroll_id_ = result_json[RES_SCROLL_ID].asString();
  }
}

bool Scroll::UpdateKeyValue(std::map<std::string, std::string> &kvs) {
  std::string scroll_time_str;
  scroll_time_str.append(std::to_string(scroll_time_));
  scroll_time_str.push_back(ScrollTimeTypeToString(time_type_));
  kvs.insert(std::make_pair(RES_SCROLL, scroll_time_str));
  if (scroll_id_.empty()) {
    kvs.insert(std::make_pair(RES_SCROLL_SEARCH_TYPE, SCROLL_TYPE_SCAN));
    return false;
  } else {
    kvs.insert(std::make_pair(RES_SCROLL_ID, scroll_id_));
    return true;
  }
  return true;
}

char Scroll::ScrollTimeTypeToString(ScrollTimeType time_type) {
  switch (time_type) {
  case aos::SCROLL_TIME_SECOND:
    return 's';
    break;
  case aos::SCROLL_TIME_MINUTE:
    return 'm';
    break;
  case aos::SCROLL_TIME_HOUR:
    return 'h';
    break;
  case aos::SCROLL_TIME_DAY:
    return 'd';
    break;
  case aos::SCROLL_TIME_WEEK:
    return 'w';
    break;
  default:
    break;
  }
  ASSERT(false);
  return 'm';
}
}  // namespace aos
