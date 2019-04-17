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

#include "ali_search/aoscontext.h"
#include <string>
#include "curl/curl.h"
#include "base/logging.h"
// #include "easylogging++.h"

// INITIALIZE_EASYLOGGINGPP

namespace aos {

AosGlobalContext::Ptr AosGlobalContext::agc_instance_;

AosGlobalContext::Ptr AosGlobalContext::InitAosGlobalContext() {
  if (!agc_instance_) {
    agc_instance_.reset(new AosGlobalContext());
    if (!agc_instance_->InitAppResouce()) {
      agc_instance_.reset();
    } else {
      agc_instance_->InitLogginpp();
    }
  }
  return agc_instance_;
}

AosGlobalContext::AosGlobalContext() {
}

AosGlobalContext::~AosGlobalContext() {
  // Uinit the curl library
  curl_global_cleanup();
}

bool AosGlobalContext::InitLogginpp() {
  // // Load configuration from file
  // el::Configurations conf("./loggin.conf");
  // // Reconfigure single logger
  // el::Loggers::reconfigureLogger("default", conf);
  // // Actually reconfigure all loggers instead
  // el::Loggers::reconfigureAllLoggers(conf);
  return true;
}

bool AosGlobalContext::InitAppResouce() {
  // Init the curl library
  CURLcode cur_res = curl_global_init(CURL_GLOBAL_ALL);
  if (cur_res != CURLcode::CURLE_OK) {
    LOG_ERROR << "curl_global_init failed " << curl_easy_strerror(cur_res);
    return false;
  }
  // Init the rand seeds
  srand((unsigned int)time(NULL));
  return true;
}

//////////////////////////////////////////////////////////////////////////////
// global curl write callback function
static size_t write_callback(void *data,
  size_t size, size_t nmemb, void *userp) {
  int size_data = size * nmemb;
  ((std::string *)(userp))->append((const char *)data, size_data);
  return size_data;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
  std::string *pdata = (std::string *)(userp);

  if (size * nmemb < 1)
    return 0;
  if (size * nmemb != pdata->size()) {
    return 0;
  }
  // Use reinterpret_cast<void *>
  ptr = (void *)(pdata->c_str()); // NOLINT
  return pdata->size();
}
//////////////////////////////////////////////////////////////////////////////

bool AosGlobalContext::SyncProcessGetRequest(
  const std::string &url, std::string &rep) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rep);
    // Perform the request, res will get the return code
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    // Check for errors
    if (res != CURLE_OK) {
      LOG_ERROR << "curl_easy_perform() failed: " << curl_easy_strerror(res);
      rep.clear();
      return false;
    }
    return true;
  }
  return false;
}

bool AosGlobalContext::SyncProcessPostRequest(const std::string &url,
    const std::string &data, std::string &rep) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    /* we want to use our own read function */
    // curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    /* pointer to pass to our read function */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rep);
    // setting the content type

    struct curl_slist *slist = 0;
    slist = curl_slist_append(
      slist, "Content-Type: application/x-www-form-urlencoded");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    // Perform the request, res will get the return code
    res = curl_easy_perform(curl);

    curl_slist_free_all(slist);
    curl_easy_cleanup(curl);
    // Check for errors
    if (res != CURLE_OK) {
      LOG_ERROR << "curl_easy_perform() failed: " << curl_easy_strerror(res);
      rep.clear();
      return false;
    }
    return true;
  }
  return false;
}

}  // namespace aos
