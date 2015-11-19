#include "ali_search/ali_search.h"
#include "curl/curl.h"

namespace aos{

  AliOpenSearch::Ptr AliOpenSearch::CreateAliOpenSearch(
    const std::string &access_key_id,
    const std::string &access_key_secret){
    AliOpenSearch::Ptr aosp(new AliOpenSearch(access_key_id, access_key_secret));
    return aosp;
  }

  AliOpenSearch::AliOpenSearch(const std::string &access_key_id,
    const std::string &access_key_secret)
    :ppm_(new PublicPartManager(access_key_id, access_key_secret)){

  }

  AliOpenSearch::~AliOpenSearch(){
    // Uinit the curl library
    curl_global_cleanup();
  }

  //bool AliOpenSearch::InitAliSearch(){

  //  // Init the curl library
  //  CURLcode cur_res = curl_global_init(CURL_GLOBAL_ALL);
  //  if (cur_res != CURLcode::CURLE_OK){
  //    LOG_ERROR << "global init libcurl error";
  //    return false;
  //  }
  //  return true;
  //}

  void AliOpenSearch::CreateNewApp(const std::string &app_name,
    const std::string &template_name){

  }

}