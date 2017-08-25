CMAKE_MINIMUM_REQUIRED(VERSION 2.6)           # CMake 最低版本要求，低于则构建过程会被终止。 
PROJECT(frame)

AUX_SOURCE_DIRECTORY(./src SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ./bin)

ADD_DEFINITIONS(-g -O0 -W -Wall -Wno-unused -DLINUX -Werror=return-type -Werror=nonnull -Werror=invalid-offsetof -Wno-deprecated ${CMAKE_CXX_FLAGS})

INCLUDE_DIRECTORIES(
	./src/
	../public/include
)

LINK_DIRECTORIES(
	../public/lib
)

ADD_EXECUTABLE(frame ${SRC_LIST})

TARGET_LINK_LIBRARIES(frame pthread)
