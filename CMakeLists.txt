#F:\code\osc\aliyun_opensearch\aliyun_opensearch\builds\vs2013>cmake ../.. -DCMAK
#E_BUILD_TYPE=DEBUG -DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DJSONCPP_WITH_TE
#STS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF
#  
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(aliyun_opensearch)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

SET(PROJECT_SET_NAME_TEST "test")
SET(PROJECT_SET_NAME_LIB "lib")
SET(PROJECT_SET_NAME_THIRD_PART "third_part")
#SET(PROJECT_MAIN_SRC_LIST "")
##########################################################
## Step 1 
SET(LIB_CURL ${PROJECT_SOURCE_DIR}/third_part/curl)
SET(EASYLOGGINGPP ${PROJECT_SOURCE_DIR}/third_part/easyloggingpp)
SET(JSON_CPP ${PROJECT_SOURCE_DIR}/third_part/jsoncpp)
SET(GLOG ${PROJECT_SOURCE_DIR}/third_part/glog)

option(USE_GLOG "Set to ON to USE Glog executable." ON)
option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
	${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake)

if (COVERALLS)
	include(Coveralls)
	coveralls_turn_on_coverage()
endif()

##1. INCLUDE_DIRECTORES
MESSAGE(STATUS "Step 1 :  Add include directores")
IF(USE_GLOG)
	SET(ALIYUN_OPENSEARCH_INCLUDE_DIR
		${LIB_CURL}/include
		${PROJECT_BINARY_DIR}/third_part/curl/include/curl
		${PROJECT_BINARY_DIR}/third_part/glog
		${GLOG}/src
		${EASYLOGGINGPP}/src
		${JSON_CPP}/include
		${PROJECT_SOURCE_DIR}/src
	)
ELSE()
	SET(ALIYUN_OPENSEARCH_INCLUDE_DIR
		${LIB_CURL}/include
		${PROJECT_BINARY_DIR}/third_part/curl/include/curl
		${EASYLOGGINGPP}/src
		${JSON_CPP}/include
		${PROJECT_SOURCE_DIR}/src
	)
ENDIF()

IF(USE_GLOG)
	IF(WIN32)
		SET(GLOG_LIB_NAME glog.lib)
	ELSE()
		SET(GLOG_LIB_NAME glog.a)
	ENDIF()
ELSE()
		SET(GLOG_LIB_NAME "")
ENDIF()

IF(WIN32)
	SET(LIBCURL_LIB_NAME libcurl.lib)
ELSEIF(APPLE)
	SET(LIBCURL_LIB_NAME libcurl.dylib)
ELSE()
	SET(LIBCURL_LIB_NAME libcurl.so)
ENDIF()

##2 LIBRARY PATH
MESSAGE(STATUS "Step 2 :  Add library path and public link library")
IF(WIN32)
	MESSAGE(STATUS "       WINDOWS")
	SET(ALIYUN_OPENSEARCH_LIBRARY_DIR
		${PROJECT_SOURCE_DIR}/lib/win
		)
	SET(ALIYUN_OPENSEARCH_LINK_LIB
		libcurl_imp.lib
		jsoncpp.lib
		winmm.lib
		${GLOG_LIB_NAME}
		libali_opensearch.lib
		)
ELSE()
	MESSAGE(STATUS "       LINUX")
	SET(ALIYUN_OPENSEARCH_LIBRARY_DIR
		${PROJECT_SOURCE_DIR}/lib/unix
		)
	SET(ALIYUN_OPENSEARCH_LINK_LIB
		libali_opensearch.a
		libjsoncpp.a
		${LIBCURL_LIB_NAME}
		${GLOG_LIB_NAME}
		pthread
		)
ENDIF()

##3 add defines
MESSAGE(STATUS "Step 3 :  Add More defines")
 if(WIN32)
	 ADD_DEFINITIONS(
		 -D_CRT_SECURE_NO_WARNINGS
		-DGLOG_NO_ABBREVIATED_SEVERITIES
	 )
 else()
	 ADD_DEFINITIONS(
	 )
 endif()

 
IF(USE_GLOG)
	 ADD_DEFINITIONS(
		-DGOOGLE_GLOG_LIBRARY
		-DGOOGLE_GLOG_DLL_DECL=
	 )
ENDIF()
 
INCLUDE_DIRECTORIES(${ALIYUN_OPENSEARCH_INCLUDE_DIR})
LINK_DIRECTORIES(${ALIYUN_OPENSEARCH_LIBRARY_DIR})
#####################################################################
# Step 3 :Set visual studio runtime type
if(WIN32)
	set(CompilerFlags
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			)
	foreach(CompilerFlag ${CompilerFlags})
	  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	  string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
	  #string(REPLACE "/EDITANDCONTINUE" "/SAFESEH" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()
	
################################################################
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win")
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win")
		 
	# With Release properties
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win")
else()
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/unix")

	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/unix")
	 
	# With Release properties
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/unix")
ENDIF()
##########################################################
## Setp 3
ADD_SUBDIRECTORY(third_part/curl)
ADD_SUBDIRECTORY(third_part/jsoncpp)

IF(USE_GLOG)
	ADD_SUBDIRECTORY(third_part/glog)
ENDIF()

ADD_SUBDIRECTORY(src)

# SET(SRC_LIST 
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/basedefines.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/basedefines.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/noncopyable.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/baseinclude.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/helpmethods.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/helpmethods.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/basictypes.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/timeutils.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/timeutils.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/base64.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/base64.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/sha1.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/sha1.cpp
	# #${CMAKE_CURRENT_SOURCE_DIR}/base/stringencode.h
	# #${CMAKE_CURRENT_SOURCE_DIR}/base/stringencode.cpp
	# #${CMAKE_CURRENT_SOURCE_DIR}/base/stringutils.h
	# #${CMAKE_CURRENT_SOURCE_DIR}/base/stringutils.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/logging.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/common.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/base/common.cpp
	
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/ali_search.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/ali_search.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/public_part.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/public_part.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/keyvalue.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/keyvalue.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/res_value.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/res_value.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/req_value.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/req_value.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/req_task.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/req_task.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/aoscontext.h
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/ali_search/aoscontext.cpp
	# )

# Setup the coveralls target and tell it to gather
# coverage data for all the lib sources.
# if (COVERALLS)
	# coveralls_setup(
        # "${SRC_LIST}"
        # ${COVERALLS_UPLOAD}                 
        # "${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake")
# endif()


# Add a test for the library.
enable_testing()

ADD_SUBDIRECTORY(test/basictest)
ADD_SUBDIRECTORY(test/klabeltest)
ADD_SUBDIRECTORY(test/helptest)
ADD_SUBDIRECTORY(test/agctest)
ADD_SUBDIRECTORY(test/sha1test)
ADD_SUBDIRECTORY(test/base64test)
ADD_SUBDIRECTORY(test/urlencodetest)
ADD_SUBDIRECTORY(test/appmanagertest)
ADD_SUBDIRECTORY(test/pushtest)
ADD_SUBDIRECTORY(test/searchtest)
ADD_SUBDIRECTORY(test/reindextest)
ADD_SUBDIRECTORY(test/suggesttest)
ADD_SUBDIRECTORY(test/geterrorlogtest)

ADD_SUBDIRECTORY(test/configstanzatest)
ADD_SUBDIRECTORY(test/qeurystanzatest)
ADD_SUBDIRECTORY(test/filterstanzatest)
ADD_SUBDIRECTORY(test/sortstanzatest)
ADD_SUBDIRECTORY(test/aggstanzatest)
ADD_SUBDIRECTORY(test/distinctstanzatest)
ADD_SUBDIRECTORY(test/kvpairstanzatest)
