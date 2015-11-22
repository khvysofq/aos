#ifndef ALI_OPENSEARCH_ALI_OPENSEARCH_RES_VALUE_H_
#define ALI_OPENSEARCH_ALI_OPENSEARCH_RES_VALUE_H_

#include "base/baseinclude.h"
#include "base/basictypes.h"
#include "json/json.h"

namespace aos{

  //////////////////////////////////////////////////////////////////////////////
  struct AosError{
    AosError() :error_code(0){
    }
    uint32 error_code;
    std::string message;
  };
  typedef std::vector<AosError> AosErrors;

  // For get app status
  struct Instance{
    bool out_of_limit;
  };
  struct Qps{
    bool out_of_limit;
  };
  struct Quota{
    Quota(){
      instance.out_of_limit = false;
      qps.out_of_limit = false;
    }
    Instance instance;
    Qps qps;
  };

  struct Row{
    std::string name;
    std::string type;
    std::string is_multi;
    std::string is_pk;
    std::string src;
  };

  typedef std::vector<Row> Rows;

  struct Table{
    std::string table_name;
    Rows rows;
  };

  typedef std::vector<Table> Tables;

  struct FromTable{
    Tables tables;
  };

  struct Fields{
    FromTable from_table;
  };

  struct StatusResult{
    std::string index_name;
    uint32 pv;
    uint32 doc_last_update_time;
    uint32 total_doc_num;
    Quota quota;
    Fields fields;
  };

  //////////////////////////////////////////////////////////////////////////////

  class ResValue{
  public:
    typedef std::shared_ptr<ResValue> Ptr;
    ResValue();
    virtual ~ResValue();

    bool ParseResult();
    // public interface
    std::string &rep_buffer(){ return rep_buffer_; }
    Json::Value &rep_json() { return json_res_; }
    bool IsSucceed() { return status_; }
    const AosErrors &GetErrorMessage() const{ return aos_errors_; }
    const std::string &get_req_id(){ return req_id_; }
    double search_time(){ return search_time_; }

  private:
    bool ParsePublicResult(Json::Value &json_res);
    virtual bool ParsePrivateResult(Json::Value &json_res){
      return true;
    }

    bool ParseErrorMessage(Json::Value &error_res);
  protected:
    std::string rep_buffer_;
    Json::Value json_res_;
    bool status_;
    AosErrors aos_errors_;
    std::string req_id_;
    double search_time_;
  };

  //////////////////////////////////////////////////////////////////////////////
  class AppStatusResValue : public ResValue{
  public:
    typedef std::shared_ptr<AppStatusResValue> Ptr;
    AppStatusResValue();
    StatusResult &Result() { return result_; }
    virtual ~AppStatusResValue();
  private:
    virtual bool ParsePrivateResult(Json::Value &json_res);
    // parse the table
    void ParseTable(Table &table, 
      const std::string &table_name, 
      Json::Value &table_json);
    // parse the table row
    void ParseRow(Row &row, const std::string &row_name, Json::Value &row_json);
  private:
    StatusResult result_;
  };

  //////////////////////////////////////////////////////////////////////////////
}

#endif // ALI_OPENSEARCH_ALI_OPENSEARCH_RES_VALUE_H_