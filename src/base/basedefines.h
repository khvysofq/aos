#ifndef ALI_OPENSEARCH_BASE_BASEDEFINES_H_
#define ALI_OPENSEARCH_BASE_BASEDEFINES_H_

#include <string>
#include <memory>

namespace aos{
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