#include "ali_search/res_value.h"
#include "base/common.h"
#include "base/basedefines.h"
#include "base/logging.h"

namespace aos{

  ResValue::ResValue() :status_(false), search_time_(0){

  }

  ResValue::~ResValue(){

  }

  bool ResValue::ParseResult(){
    if (rep_buffer_.empty()){
      LOG_ERROR << "The rep buffer is null, you shouldn't parse the result";
      return false;
    }
    DLOG_INFO << rep_buffer_;
    // parse the json value
    Json::Reader rep_reader;
    if (!rep_reader.parse(rep_buffer_, json_res_)){
      LOG_ERROR << rep_reader.getFormattedErrorMessages();
      return false;
    }
    return ParsePublicResult(json_res_);
  }

  bool ResValue::ParsePublicResult(Json::Value &json_res){
    // 
    if (json_res.isNull()){
      LOG_ERROR << "The result is null";
      return false;
    }
    // Get the request id
    req_id_ = json_res[JSON_REQUEST_ID].asString();
    if (req_id_.empty()){
      LOG_WARNING << "req_id is null";
    }
    if (!json_res[JSON_SEARCH_TIME].isNull()){
      search_time_ = json_res[JSON_SEARCH_TIME].asDouble();
    }
    // Get result status
    std::string status_str = json_res[JSON_STATUS].asString();
    if (status_str.empty()){
      status_str = JSON_STATUS_OK;
      LOG_WARNING << "the result status is empty";
    }
    if (status_str == JSON_STATUS_FALT){
      status_ = false;
      ParseErrorMessage(json_res[JSON_ERRORS]);
      return true;
    }
    else if (status_str == JSON_STATUS_OK){
      status_ = true;
      return ParsePrivateResult(json_res);
    }
    else{
      LOG_ERROR << "The result status is not the expectations value " << status_str;
      //ASSERT(false);
    }
    return false;
  }

  bool ResValue::ParseErrorMessage(Json::Value &error_res){
    if (error_res.isNull()){
      LOG_ERROR << "the error code filed is null";
      return false;
    }
    for (std::size_t i = 0; i < error_res.size(); i++){
      AosError aos_error;
      aos_error.error_code = error_res[(int)i][JSON_ERRORS_CODE].asUInt();
      aos_error.message = error_res[(int)i][JSON_ERRORS_MESSAGE].asString();
      aos_errors_.push_back(aos_error);
      LOG_ERROR << "[ " << aos_error.error_code << " ] "
        << aos_error.message;
    }
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////
  //AppStatusResValue::AppStatusResValue(){

  //}

  //AppStatusResValue::~AppStatusResValue(){

  //}

  //bool AppStatusResValue::ParsePrivateResult(Json::Value &json_res){
  //  Json::Value &result_json = json_res[JSON_RESULT];
  //  // index_name
  //  result_.index_name = result_json[JSON_RESULT_INDEX_NAME].asString();
  //  result_.pv = result_json[JSON_RESULT_PV].asUInt();
  //  result_.doc_last_update_time = result_json[JSON_RESULT_DLUT].asUInt();
  //  result_.total_doc_num = result_json[JSON_RESULT_TDN].asUInt();
  //  // quota
  //  Quota &quota = result_.quota;
  //  Json::Value &quota_json = result_json[JSON_RESULT_QUOTA];
  //  quota.instance.out_of_limit =
  //    quota_json[JSON_RESULT_QUOTA_I][JSON_RESULT_QUOTA_IOOL].asBool();
  //  quota.qps.out_of_limit =
  //    quota_json[JSON_RESULT_QPS][JSON_RESULT_QPS_OOL].asBool();
  //  //
  //  Json::Value fields_json = result_json[JSON_RESULT_FIELDS];
  //  if (fields_json.isNull()){
  //    return true;
  //  }
  //  Json::Value from_table_json = fields_json[JSON_RESULT_FIELDS_FT];
  //  if (from_table_json.isNull()){
  //    return true;
  //  }
  //  Json::Value tables_json = from_table_json[JSON_RESULT_FIELDS_FT_T];
  //  if (from_table_json.isNull()){
  //    return true;
  //  }
  //  LOG_INFO << json_res.toStyledString();
  //  Json::Value::Members table_members = tables_json.getMemberNames();
  //  for (std::size_t i = 0; i < table_members.size(); i++){
  //    Table table;
  //    ParseTable(table, table_members[i], tables_json[table_members[i]]);
  //    result_.fields.from_table.tables.push_back(table);
  //  }
  //  return true;
  //}

  //void AppStatusResValue::ParseTable(Table &table,
  //  const std::string &table_name,
  //  Json::Value &table_json){
  //  //
  //  if (table_json.isNull()){
  //    return;
  //  }
  //  table.table_name = table_name;
  //  Json::Value::Members rows_names = table_json.getMemberNames();
  //  for (std::size_t i = 0; i < rows_names.size(); i++){
  //    Row row;
  //    ParseRow(row, rows_names[i], table_json[rows_names[i]]);
  //    table.rows.push_back(row);
  //  }
  //}

  //void AppStatusResValue::ParseRow(Row &row, const std::string &row_name,
  //  Json::Value &row_json){
  //  row.name = row_name;
  //  std::string name = row_json[JSON_RESULT_TABLE_ROW_NAME].asString();
  //  ASSERT(row_name == name);
  //  row.type = row_json[JSON_RESULT_TABLE_ROW_TYPE].asString();
  //  row.is_multi = row_json[JSON_RESULT_TABLE_ROW_IS_MULTI].asString();
  //  row.is_pk = row_json[JSON_RESULT_TABLE_ROW_IS_PK].asString();
  //  row.src = row_json[JSON_RESULT_TABLE_ROW_SRC].asString();
  //}
}