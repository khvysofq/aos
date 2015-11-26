#include <iostream>
#include "base/baseinclude.h"
#include "base/logging.h"
//#include "easylogging++.h"

//INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]){
  google::InitGoogleLogging(argc[0]);
  FLAGS_logtostderr = true;
  FLAGS_stderrthreshold = 0;
  FLAGS_colorlogtostderr = true;
  std::cout << "Hello World" << std::endl;

  LOG_INFO << "Hello World";
  LOG_WARNING << "Hello World";
  LOG_ERROR << "Hello World";
  return 0;
}