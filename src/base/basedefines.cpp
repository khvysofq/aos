#include "base/basedefines.h"

namespace aos{
  // Common mistakes
  AOS_ERROR_MAP_DEF(INTERNAL_SERVER_ERROR, 1000, "Format mistaks or timeout");
  AOS_ERROR_MAP_DEF(SORT_QUERY_PARAMER_ERROR, 2112, "sort expression warning");
  AOS_ERROR_MAP_DEF(PUSH_FREQUENCY_LIMIT_WARNING, 3007, "Push is too frequently");
  AOS_ERROR_MAP_DEF(AUTHORIZE_ERROR, 4003, "Authorize error");
  AOS_ERROR_MAP_DEF(JSON_FORMAT_ERROR, 4007, "Json format error");
  AOS_ERROR_MAP_DEF(TIME_STAMP_FORMAT_ERROR, 4010, "TimeStamp format error");
  AOS_ERROR_MAP_DEF(NOT_FOUND_THE_USER, 5001, "The user not found");
  AOS_ERROR_MAP_DEF(ACCESS_KEY_AUTHORIZE_ERROR, 5008, "Access key authorize error");
  AOS_ERROR_MAP_DEF(START_HIT_LIMIT, 6013, "Start and hit does not exceed 5000 characters");
  AOS_ERROR_MAP_DEF(QPS_LIMIT, 6015, "QPS limit");


  // System error defines
  AOS_ERROR_MAP_DEF(TEMPLETE_NOT_FOUND, 1001, "Not found the templete");
  AOS_ERROR_MAP_DEF(INDEX_TYPE_NOT_SUPPRT, 1003, "The index type is not support");
  AOS_ERROR_MAP_DEF(SERVICE_UNAVALIABLE , 1004, "Service currently unavailable");

  // Application layer error
  AOS_ERROR_MAP_DEF(APP_NOT_FOUND, 2001, "The find app does not exist");
  AOS_ERROR_MAP_DEF(APP_ALREADY_EXISTS, 2002, "The app already exists");
  AOS_ERROR_MAP_DEF(REACH_APP_LIMITS, 2003, "Reach app count limit");
  AOS_ERROR_MAP_DEF(APP_NAME_FORMAT_ERROR, 2004, "App name format error");
  AOS_ERROR_MAP_DEF(APP_NAME_NOT_SET, 2005, "App name is not set");
  AOS_ERROR_MAP_DEF(NEW_APP_NAME_NOT_SET, 2006, "New app name is not set");
  AOS_ERROR_MAP_DEF(NODE_SIZE_LIMIT, 2007, "Note does not exceed 300 characters");
  AOS_ERROR_MAP_DEF(SUMMARY_CFG_ERR, 2008, "Parameters of confgiureation error");
  AOS_ERROR_MAP_DEF(UPDATE_STATE_ERR, 2009, "Update state error");
  AOS_ERROR_MAP_DEF(APP_IN_THE_PAUSE, 2010, "App in the pause state");

}