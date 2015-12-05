#include "ali_search/ali_search.h"
#include "base/basedefines.h"
#include "base/logging.h"
#include "ali_search/res_value.h"
//#include "easylogging++.h"

//INITIALIZE_EASYLOGGINGPP
//
#define GKD(NAME)\
  aos::GET_KLABEL_DESCRIBE(NAME);
#define GKV(NAME)\
  aos::GET_KLABEL_VALUE(NAME)

int main(int argv, char* argc[]){
  
  aos::ResValue::Ptr res_value(new aos::ResValue());
  res_value->ParseResult();
  res_value->rep_buffer() = "AFDSFSDLJVLDSKJFHF:LEKFJLDFDSFSD";
  res_value->ParseResult();
  res_value->rep_buffer() = "{}";
  res_value->ParseResult();

  return 0;
}