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

##1. INCLUDE_DIRECTORES
if(UNIX)
	SET(ALIYUN_OPENSEARCH_INCLUDE_DIR
	${LIB_CURL}/include
	${LIB_CURL}/builds/vs2013/include/curl
	${EASYLOGGINGPP}/src
	${JSON_CPP}/include
	${PROJECT_SOURCE_DIR}/src
	)
elseif(WIN32)
	SET(ALIYUN_OPENSEARCH_INCLUDE_DIR
	${LIB_CURL}/include
	${PROJECT_BINARY_DIR}/third_part/curl/include/curl
	${EASYLOGGINGPP}/src
	${JSON_CPP}/include
	${PROJECT_SOURCE_DIR}/src
	)
endif()

MESSAGE(STATUS "       Add library path and name")
IF(UNIX)
	MESSAGE(STATUS "       LINUX")
	SET(ALIYUN_OPENSEARCH_LIBRARY_DIR
		${JSON_CPP}/builds/vs2013/src/lib_json/Debug
		)
	SET(ALIYUN_OPENSEARCH_LINK_LIB
		-std=c++11
		pthread
		rt
		)
ELSEIF(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES DEBUG)
		MESSAGE(STATUS "       Debug Mode")
		MESSAGE(STATUS "       MSVC12")
		SET(ALIYUN_OPENSEARCH_LIBRARY_DIR
			${PROJECT_SOURCE_DIR}/lib/win32/Debug
			)
		SET(ALIYUN_OPENSEARCH_LINK_LIB
			libcurl_imp.lib
			jsoncpp.lib
			winmm.lib
			)
	elseif(CMAKE_BUILD_TYPE MATCHES RELEASE)
		MESSAGE(STATUS "       Release Mode")
		MESSAGE(STATUS "       MSVC12")
		SET(ALIYUN_OPENSEARCH_LIBRARY_DIR
			${PROJECT_SOURCE_DIR}/lib/win32/Release
			)
		SET(ALIYUN_OPENSEARCH_LINK_LIB
			libcurl_imp.lib
			jsoncpp.lib
			winmm.lib
			)
	endif()
ENDIF()

MESSAGE(STATUS "Step 4 : Add code source")
# if(UNIX)
	# ADD_DEFINITIONS()
# elseif(WIN32)
	# ADD_DEFINITIONS(
	# )
# endif()

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
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/win32")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win32")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win32")
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/bin/win32/Debug")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win32/Debug")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win32/Debug")
		 
	# With Release properties
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/bin/win32/Release")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win32/Release")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win32/Release")
elseif(UNIX)
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
ADD_SUBDIRECTORY(test/basictest)
ADD_SUBDIRECTORY(test/klabeltest)
ADD_SUBDIRECTORY(test/helpteset)


ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(third_part/curl)
ADD_SUBDIRECTORY(third_part/jsoncpp)