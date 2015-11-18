#include <iostream>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]){
  el::Configurations defaultConf;
  defaultConf.setToDefault();
  // Values are always std::string
  defaultConf.set(el::Level::Info,
    el::ConfigurationType::Format, "%datetime %level %msg");
  // default logger uses default configurations
  el::Loggers::reconfigureLogger("default", defaultConf);

  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

  LOG(INFO) << "Log using default file";

  LOG(INFO) << "HELLO WORLD";
  LOG(WARNING) << "HELLO WORLD";
  LOG(ERROR) << "HELLO WORLD";
  return 0;
}