#include <iostream>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[]){
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  el::Configurations defaultConf;
  defaultConf.setToDefault();
  // Values are always std::string
  defaultConf.set(el::Level::Global,
    el::ConfigurationType::Format, "[%level %datetime %fbase %line] %msg");
  defaultConf.parseFromText("*GLOBAL:\n FORMAT = %level %msg");
  // default logger uses default configurations
  el::Loggers::reconfigureLogger("default", defaultConf);
  LOG(INFO) << "Log using default file";

  LOG(INFO) << "Log using default file";

  LOG(INFO) << "HELLO WORLD";
  LOG(WARNING) << "HELLO WORLD";
  LOG(ERROR) << "HELLO WORLD";
  return 0;
}