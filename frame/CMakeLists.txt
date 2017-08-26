 
PROJECT(frame)

AUX_SOURCE_DIRECTORY(./src SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ./bin)
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
MESSAGE(STSTUS "###################################")
IF (WIN32)
    MESSAGE(STATUS "Now is windows")
    LINK_DIRECTORIES(../public/lib/win)
ELSEIF (APPLE)
    MESSAGE(STATUS "Now is Apple system")
    LINK_DIRECTORIES(../public/lib/mac)
ELSEIF (LINUX)
	MESSAGE(STATUS "Now is Linux system")
ENDIF ()


add_compile_options(-std=c++11)
ADD_DEFINITIONS(-g -O0 -W -Wall -Wno-unused -DLINUX -Werror=return-type -Werror=nonnull -Werror=invalid-offsetof -Wno-deprecated ${CMAKE_CXX_FLAGS})

INCLUDE_DIRECTORIES(
	./src/
	../public/include
)

LINK_DIRECTORIES(
	../public/lib
)

ADD_EXECUTABLE(frame ${SRC_LIST})

TARGET_LINK_LIBRARIES(frame pthread tinyxml2)
