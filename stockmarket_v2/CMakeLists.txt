CMAKE_MINIMUM_REQUIRED(VERSION 2.6)           # CMake 最低版本要求，低于则构建过程会被终止。 
PROJECT(stockmarket_v2)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
AUX_SOURCE_DIRECTORY(../public/src SRC_LIST)
AUX_SOURCE_DIRECTORY(../public/src/timer SRC_LIST)
AUX_SOURCE_DIRECTORY(../public/src/curlhelp SRC_LIST)
AUX_SOURCE_DIRECTORY(../public/src/file_flow SRC_LIST)
AUX_SOURCE_DIRECTORY(../public/src/protocal SRC_LIST)
AUX_SOURCE_DIRECTORY(../public/src/util SRC_LIST)
AUX_SOURCE_DIRECTORY(../public/src/zookeeper_cli SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ./bin)

SET(CMAKE_CXX_COMPILER g++)

#add_compile_options(-std=c++11)
#add_compile_options(-std=c++11)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ADD_DEFINITIONS(-std=c++11 -g -Wreorder -DUSE_SPDLOG -DTHREADED -DZMQ_BUILD_DRAFT_API -DSPDLOG_TRACE_ON -DLINUX -Werror=return-type -Werror=nonnull -Werror=invalid-offsetof -Wno-deprecated ${CMAKE_CXX_FLAGS})

INCLUDE_DIRECTORIES(
	./src/
	../public/include
        ../public/src/
	../public/src/timer
	../public/src/curlhelp
	../public/src/file_flow
	../public/src/protocal
	../public/src/util
	../public/include/mysql-connector
        ../public/include/zookeeper
)

LINK_DIRECTORIES(
	../public/lib/ubuntu
)

ADD_EXECUTABLE(stockmarket_v2 ${SRC_LIST})

TARGET_LINK_LIBRARIES(stockmarket_v2 pthread tinyxml2 curl zmq uWS ssl protobuf mysqlcppconn-static dl zookeeper_mt)
