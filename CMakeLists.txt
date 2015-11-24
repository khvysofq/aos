#F:\code\osc\aliyun_opensearch\aliyun_opensearch\builds\vs2013>cmake ../.. -DCMAK
#E_BUILD_TYPE=DEBUG -DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DJSONCPP_WITH_TE
#STS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF
#  
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(aliyun_opensearch)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

##########################################################
## Step 1 
SET(LIB_CURL ${PROJECT_SOURCE_DIR}/third_part/curl)
SET(EASYLOGGINGPP ${PROJECT_SOURCE_DIR}/third_part/easyloggingpp)
SET(JSON_CPP ${PROJECT_SOURCE_DIR}/third_part/jsoncpp)
SET(GLOG ${PROJECT_SOURCE_DIR}/third_part/glog)

##1. INCLUDE_DIRECTORES
MESSAGE(STATUS "Step 1 :  Add include directores")
SET(ALIYUN_OPENSEARCH_INCLUDE_DIR
	${LIB_CURL}/include
	${PROJECT_BINARY_DIR}/third_part/curl/include/curl
	${PROJECT_BINARY_DIR}/third_part/glog
	${EASYLOGGINGPP}/src
	${JSON_CPP}/include
	${GLOG}/src
	${PROJECT_SOURCE_DIR}/src
)

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
		glog.lib
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
		libcurl.so
		libglog.a
		pthread
		rt
		)
ENDIF()

##3 add defines
MESSAGE(STATUS "Step 3 :  Add More defines")
 if(WIN32)
	 ADD_DEFINITIONS(
		 -D_CRT_SECURE_NO_WARNINGS
		-DGOOGLE_GLOG_DLL_DECL=
		-DGLOG_NO_ABBREVIATED_SEVERITIES
	 )
 else()
	 ADD_DEFINITIONS(
		-DGOOGLE_GLOG_DLL_DECL=
	 )
 endif()

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
ADD_SUBDIRECTORY(third_part/glog)
ADD_SUBDIRECTORY(src)

ADD_SUBDIRECTORY(test/basictest)
ADD_SUBDIRECTORY(test/klabeltest)
ADD_SUBDIRECTORY(test/helptest)
ADD_SUBDIRECTORY(test/agctest)
ADD_SUBDIRECTORY(test/sha1test)
ADD_SUBDIRECTORY(test/base64test)
ADD_SUBDIRECTORY(test/urlencodetest)
ADD_SUBDIRECTORY(test/appmanagertest)
ADD_SUBDIRECTORY(test/pushtest)
ADD_SUBDIRECTORY(test/qeurystanzatest)
ADD_SUBDIRECTORY(test/searchtest)
