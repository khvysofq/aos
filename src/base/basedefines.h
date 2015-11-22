
#ifndef ALI_OPENSEARCH_BASE_BASEDEFINES_H_
#define ALI_OPENSEARCH_BASE_BASEDEFINES_H_

#include <string>

namespace aos{


  // Public request filed name defines
  const char REQ_VERSION[] = "Version";
  const char REQ_ACCESS_KEY_ID[] = "AccessKeyId";
  const char REQ_SIGNATURE[] = "Signature";
  const char REQ_SIGNATURE_METHOD[] = "SignatureMethod";
  const char REQ_TIMESTAMP[] = "Timestamp";
  const char REQ_SIGNATURE_VERSION[] = "SignatureVersion";
  const char REQ_SIGNATURE_NONCE[] = "SignatureNonce";

  // Public const defines
  const char ALI_OPENSEARCH_PROTO_VERSION[] = "v2";
  const char ALI_OPENSEARCH_SIGNATURE_METHOD_HMAC_SHA1[] = "HMAC-SHA1";
  const char ALI_OPENSEARCH_SIGNATURE_VERION[] = "1.0";

  // Private request filed name defines
  const char REQ_ACTION[] = "action";
  const char REQ_TEMPLATE[] = "template";
  const char REQ_QUERY[] = "query";
  const char REQ_INDEX_NAME[] = "index_name";
  const char REQ_FORMAT[] = "format";
  const char REQ_FETCH_FIELDS[] = "fetch_fields";

  // With application manager process
  const char APP_MANAGER_URL[] = "/index";
  const char APP_MANAGER_ACTION_CREATE[] = "create";
  const char APP_MANAGER_ACTION_DELETE[] = "delete";
  const char APP_MANAGER_ACTION_STATUS[] = "status";

  // Suppert builtin template name
  const char APP_TEMPLATE_BUILTIN_NEWS[] = "builtin_news";
  const char APP_TEMPLATE_BUILTIN_NOVEL[] = "builtin_novel";
  const char APP_TEMPLATE_BUILTIN_DOWNLOAD[] = "builtin_download";
  const char APP_TEMPLATE_BUILTIN_BBS[] = "bbs";

  // application manager json value
  const char JSON_STATUS[] = "status";
  const char JSON_STATUS_FALT[] = "FAIL";
  const char JSON_STATUS_OK[] = "OK";
  const char JSON_ERRORS[] = "errors";
  const char JSON_ERRORS_CODE[] = "code";
  const char JSON_ERRORS_MESSAGE[] = "message";
  const char JSON_REQUEST_ID[] = "request_id";
  const char JSON_SEARCH_TIME[] = "searchtime";

  const char JSON_RESULT[] = "result";
  const char JSON_RESULT_INDEX_NAME[] = "index_name";
  const char JSON_RESULT_PV[] = "pv";
  const char JSON_RESULT_DLUT[] = "doc_last_update_time";
  const char JSON_RESULT_TDN[] = "total_doc_num";
  const char JSON_RESULT_QUOTA[] = "quota";
  const char JSON_RESULT_QUOTA_I[] = "instance";
  const char JSON_RESULT_QUOTA_IOOL[] = "out_of_limit";
  const char JSON_RESULT_QPS[] = "qps";
  const char JSON_RESULT_QPS_OOL[] = "out_of_limit";
  const char JSON_RESULT_FIELDS[] = "fields";
  const char JSON_RESULT_FIELDS_FT[] = "from_table";
  const char JSON_RESULT_FIELDS_FT_T[] = "tables";
  const char JSON_RESULT_TABLE_ROW_NAME[] = "name";
  const char JSON_RESULT_TABLE_ROW_TYPE[] = "type";
  const char JSON_RESULT_TABLE_ROW_IS_MULTI[] = "is_multi";
  const char JSON_RESULT_TABLE_ROW_IS_PK[] = "is_pk";
  const char JSON_RESULT_TABLE_ROW_SRC[] = "src";
  //const char JSON_RESULT_FIELDS_FT_T_M[] = "main";
  //const char JSON_RESULT_FIELDS_FT_T_M_I[] = "id";
  //const char JSON_RESULT_FIELDS_FT_T_M_I_

  // push doc to server
  const char APP_PUSH_URL[] = "/index/doc";
  const char RES_ACTION_PUSH[] = "push";
  const char RES_TABLE_NAME[] = "table_name";
  const char JSON_PUSH_CMD[] = "cmd";
  const char JSON_PUSH_CMD_ADD[] = "add";
  const char JSON_PUSH_CMD_UPDATE[] = "update";
  const char JSON_PUSH_CMD_DELETE[] = "delete";
  const char JSON_PUSH_FIELDS[] = "fields";
  const char JSON_PUSH_ITEM_ID[] = "id";
  const char JSON_PUSH_TIMESTAMP[] = "timestamp";
  const char JSON_PUSH_ITEMS[] = "items";

  // Suggest 
  const char APP_SUGGEST_URL[] = "/suggest";
  const char RES_SUGGEST_QUERY[] = "query";
  const char RES_SUGGEST_INDEX_NAME[] = "index_name";
  const char RES_SUGGEST_NAME[] = "suggest_name";
  const char RES_SUGGEST_HIT[] = "hit";

  // Reindex 
  const char APP_REINDEX_URL[] = "/index";
  const char RES_REINDEX_ACTION_CREATATASK[] = "action";
  const char RES_REINDEX_OPERATE[] = "operate";
  const char RES_REINDEX_OPERATE_IMPORT[] = "import";
  const char RES_REINDEX_TABLE_NAME[] = "table_name";

  // Get error log
  const char APP_GET_ERROR_LOG_URL[] = "/index/error";
  const char RES_GEL_PAGE[] = "page";
  const char RES_GEL_PAGE_SIZE[] = "page_size";
  const char RES_GEL_SORT_MODE[] = "sort_mode";
  const char RES_GEL_SORT_MODE_ASC[] = "ASC";
  const char RES_GEL_SORT_MODE_DESC[] = "DESC";

  // Search
  const char RES_SUMMARY_PREFIX[] = "summary_prefix";
  const char RES_SUMMARY_POSTFIX[] = "summary_postfix";
  const char RES_SUMMARY_LEN[] = "summary_len";
  const char RES_SUMMARY_SNIPPED[] = "summary_snipped";
  const char RES_SUMMARY_ELLIPSIS[] = "summary_ellipsis";
  const char RES_SUMMARY_ELEMENT[] = "summary_element";
  const char RES_SUMMARY_FIELD[] = "summary_field";

  // query
  const char DEFAULT_INDEX_NAME[] = "defualt";
  const char UNION_TYPE_AND[] = "AND";
  const char UNION_TYPE_OR[] = "OR";
  const char UNION_TYPE_ANDNOT[] = "ANDNOT";
  const char UNION_TYPE_RANK[] = "RANK";

  // Query aggregate
  const char QUERY_AGGREGATE_RANGE[] = "range";
  const char QUERY_AGGREGATE_GROUP_KEY[] = "group_key";
  const char QUERY_AGGREGATE_FUN[] = "agg_fun";
  const char QUERY_AGGREGATE_MAX_GROUP[] = "max_group";
  const char QUERY_AGGREGATE_FILTER[] = "agg_filter";
  const char QUERY_AGGREGATE_AST[] = "agg_sampler_threshold";
  const char QUERY_AGGREGATE_ASS[] = "agg_sampler_step";



#define AOS_ERROR_MAP_DEF(NAME, NCODE, DESCRIBE)\
  const int ERROR_##NCODE = NCODE;\
  const int NAME = NCODE;\
  const char NAME##STR[] = "error code [ " #NCODE " ] " DESCRIBE;

#define GET_KLABEL_DESCRIBE(NAME) NAME##STR
#define GET_KLABEL_VALUE(NAME) NAME

#define EXTERN_AOS_SYMBOL(NAME)\
  extern const int NAME;\
  extern const char NAME##STR[];\

  EXTERN_AOS_SYMBOL(INTERNAL_SERVER_ERROR);
  EXTERN_AOS_SYMBOL(SORT_QUERY_PARAMER_ERROR);
  EXTERN_AOS_SYMBOL(PUSH_FREQUENCY_LIMIT_WARNING);
  EXTERN_AOS_SYMBOL(AUTHORIZE_ERROR);
  EXTERN_AOS_SYMBOL(JSON_FORMAT_ERROR);
  EXTERN_AOS_SYMBOL(TIME_STAMP_FORMAT_ERROR);
  EXTERN_AOS_SYMBOL(NOT_FOUND_THE_USER);
  EXTERN_AOS_SYMBOL(ACCESS_KEY_AUTHORIZE_ERROR);
  EXTERN_AOS_SYMBOL(START_HIT_LIMIT);
  EXTERN_AOS_SYMBOL(QPS_LIMIT);


  // System error defines
  EXTERN_AOS_SYMBOL(TEMPLETE_NOT_FOUND);
  EXTERN_AOS_SYMBOL(INDEX_TYPE_NOT_SUPPRT);
  EXTERN_AOS_SYMBOL(SERVICE_UNAVALIABLE);

  // Application layer error
  EXTERN_AOS_SYMBOL(APP_NOT_FOUND);
  EXTERN_AOS_SYMBOL(APP_ALREADY_EXISTS);
  EXTERN_AOS_SYMBOL(REACH_APP_LIMITS);
  EXTERN_AOS_SYMBOL(APP_NAME_FORMAT_ERROR);
  EXTERN_AOS_SYMBOL(APP_NAME_NOT_SET);
  EXTERN_AOS_SYMBOL(NEW_APP_NAME_NOT_SET);
  EXTERN_AOS_SYMBOL(NODE_SIZE_LIMIT);
  EXTERN_AOS_SYMBOL(SUMMARY_CFG_ERR);
  EXTERN_AOS_SYMBOL(UPDATE_STATE_ERR);
  EXTERN_AOS_SYMBOL(APP_IN_THE_PAUSE);
}

#endif // ALI_OPENSEARCH_BASE_BASEDEFINES_H_