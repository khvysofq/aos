#include "ali_search/public_part.h"
#include "base/basedefines.h"
#include "base/helpmethods.h"

namespace aos{

  PublicPartManager::PublicPartManager(const std::string &api_domain, 
    const std::string &access_key_id,
    const std::string &access_key_secret) 
    :access_key_secret_(access_key_secret + "&"),
    api_domain_(api_domain){
    InitPublicPart(access_key_id, access_key_secret);
  }

  PublicPartManager::~PublicPartManager(){

  }

  void PublicPartManager::InitPublicPart(const std::string &access_key_id,
    const std::string &access_key_secret){
    // 1. Insert AccessKeyId
    keyvalues_.insert(std::make_pair(REQ_VERSION, ALI_OPENSEARCH_PROTO_VERSION));
    // 2. Insert SignatureMethod
    keyvalues_.insert(std::make_pair(REQ_ACCESS_KEY_ID, access_key_id));
    ////////////////////////////////////////////////////////////////////////////
    // 3. Insert Signature
    // keyvalues_.insert(std::make_pair(REQ_SIGNATURE, access_key_secret));
    ////////////////////////////////////////////////////////////////////////////
    // 4. Insert SignatureMethod
    keyvalues_.insert(std::make_pair(REQ_SIGNATURE_METHOD, 
      ALI_OPENSEARCH_SIGNATURE_METHOD_HMAC_SHA1));
    // 5. Insert Timestamp
    keyvalues_.insert(std::make_pair(REQ_TIMESTAMP, ""));
    // 6. Insert SignatureVersion
    keyvalues_.insert(std::make_pair(REQ_SIGNATURE_VERSION,
      ALI_OPENSEARCH_SIGNATURE_VERION));
    // 7. Insert SignatureNonce
    keyvalues_.insert(std::make_pair(REQ_SIGNATURE_NONCE,""));
  }

  // Thread safe function
  void PublicPartManager::CopyPublicPart(std::map<std::string, std::string> &kvs){

    std::lock_guard<std::mutex> lock_copy(kvs_mutex_);
    // copy the public keys
    kvs = keyvalues_;
    kvs[REQ_TIMESTAMP] = HelpMethos::GetCurrentUTCTime();
    kvs[REQ_SIGNATURE_NONCE] = HelpMethos::GetSignatureNonce();
    //kvs[REQ_TIMESTAMP] = "2014-07-14T01:34:55Z";
    //kvs[REQ_SIGNATURE_NONCE] = "14053016951271226";
  }
}