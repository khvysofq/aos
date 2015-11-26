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
}

#endif // ALI_OPENSEARCH_ALI_OPENSEARCH_RES_VALUE_H_