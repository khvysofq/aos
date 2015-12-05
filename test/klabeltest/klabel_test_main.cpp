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

int main(int argv, char* argc[]) {

  aos::ResValue::Ptr res_value(new aos::ResValue());
  res_value->ParseResult();
  res_value->rep_buffer() = "AFDSFSDLJVLDSKJFHF:LEKFJLDFDSFSD";
  res_value->ParseResult();
  res_value->rep_buffer() = "{}";
  res_value->ParseResult();

  return 0;
}