#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Convert.o \
	${OBJECTDIR}/CurlHelp.o \
	${OBJECTDIR}/application.o \
	${OBJECTDIR}/depthmarketdata.pb.o \
	${OBJECTDIR}/flow_worker.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/md_engine.o \
	${OBJECTDIR}/md_pubber.o \
	${OBJECTDIR}/public/src/timer/clock.o \
	${OBJECTDIR}/public/src/timer/epoll.o \
	${OBJECTDIR}/public/src/timer/poller_base.o \
	${OBJECTDIR}/public/src/timer/signaler.o \
	${OBJECTDIR}/public/src/timer/thread.o \
	${OBJECTDIR}/sqlconnector.o \
	${OBJECTDIR}/src/file_flow/cache_flow.o \
	${OBJECTDIR}/src/file_flow/cache_list.o \
	${OBJECTDIR}/src/file_flow/file_flow.o \
	${OBJECTDIR}/src/file_flow/flow.o \
	${OBJECTDIR}/src/file_flow/mpath.o \
	${OBJECTDIR}/src/file_flow/platform.o \
	${OBJECTDIR}/src/file_flow/read_only_flow.o \
	${OBJECTDIR}/src/file_flow/system.o \
	${OBJECTDIR}/src/log.o \
	${OBJECTDIR}/src/mailbox_event.o \
	${OBJECTDIR}/src/mdstock_collector.o \
	${OBJECTDIR}/src/thread.o \
	${OBJECTDIR}/src/timer.o \
	${OBJECTDIR}/src/util.o \
	${OBJECTDIR}/src/zmq_poller_reactor.o \
	${OBJECTDIR}/stcode_updator.o \
	${OBJECTDIR}/update_thread.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../lib -Wl,-rpath,'../../lib' public/lib/libtinyxml2.a ../../lib/libzmq.so

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stockmarket_v2

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stockmarket_v2: public/lib/libtinyxml2.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stockmarket_v2: ../../lib/libzmq.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stockmarket_v2: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stockmarket_v2 ${OBJECTFILES} ${LDLIBSOPTIONS} -luWS -lssl -lmysqlcppconn -lcurl -lprotobuf

${OBJECTDIR}/Convert.o: Convert.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Convert.o Convert.cpp

${OBJECTDIR}/CurlHelp.o: CurlHelp.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CurlHelp.o CurlHelp.cpp

${OBJECTDIR}/application.o: application.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/application.o application.cpp

${OBJECTDIR}/depthmarketdata.pb.o: depthmarketdata.pb.cc
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/depthmarketdata.pb.o depthmarketdata.pb.cc

${OBJECTDIR}/flow_worker.o: flow_worker.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/flow_worker.o flow_worker.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/md_engine.o: md_engine.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/md_engine.o md_engine.cpp

${OBJECTDIR}/md_pubber.o: md_pubber.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/md_pubber.o md_pubber.cpp

${OBJECTDIR}/public/src/timer/clock.o: public/src/timer/clock.cpp
	${MKDIR} -p ${OBJECTDIR}/public/src/timer
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public/src/timer/clock.o public/src/timer/clock.cpp

${OBJECTDIR}/public/src/timer/epoll.o: public/src/timer/epoll.cpp
	${MKDIR} -p ${OBJECTDIR}/public/src/timer
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public/src/timer/epoll.o public/src/timer/epoll.cpp

${OBJECTDIR}/public/src/timer/poller_base.o: public/src/timer/poller_base.cpp
	${MKDIR} -p ${OBJECTDIR}/public/src/timer
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public/src/timer/poller_base.o public/src/timer/poller_base.cpp

${OBJECTDIR}/public/src/timer/signaler.o: public/src/timer/signaler.cpp
	${MKDIR} -p ${OBJECTDIR}/public/src/timer
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public/src/timer/signaler.o public/src/timer/signaler.cpp

${OBJECTDIR}/public/src/timer/thread.o: public/src/timer/thread.cpp
	${MKDIR} -p ${OBJECTDIR}/public/src/timer
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public/src/timer/thread.o public/src/timer/thread.cpp

${OBJECTDIR}/sqlconnector.o: sqlconnector.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sqlconnector.o sqlconnector.cpp

${OBJECTDIR}/src/file_flow/cache_flow.o: src/file_flow/cache_flow.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/cache_flow.o src/file_flow/cache_flow.cpp

${OBJECTDIR}/src/file_flow/cache_list.o: src/file_flow/cache_list.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/cache_list.o src/file_flow/cache_list.cpp

${OBJECTDIR}/src/file_flow/file_flow.o: src/file_flow/file_flow.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/file_flow.o src/file_flow/file_flow.cpp

${OBJECTDIR}/src/file_flow/flow.o: src/file_flow/flow.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/flow.o src/file_flow/flow.cpp

${OBJECTDIR}/src/file_flow/mpath.o: src/file_flow/mpath.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/mpath.o src/file_flow/mpath.cpp

${OBJECTDIR}/src/file_flow/platform.o: src/file_flow/platform.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/platform.o src/file_flow/platform.cpp

${OBJECTDIR}/src/file_flow/read_only_flow.o: src/file_flow/read_only_flow.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/read_only_flow.o src/file_flow/read_only_flow.cpp

${OBJECTDIR}/src/file_flow/system.o: src/file_flow/system.cpp
	${MKDIR} -p ${OBJECTDIR}/src/file_flow
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/file_flow/system.o src/file_flow/system.cpp

${OBJECTDIR}/src/log.o: src/log.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/log.o src/log.cpp

${OBJECTDIR}/src/mailbox_event.o: src/mailbox_event.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mailbox_event.o src/mailbox_event.cpp

${OBJECTDIR}/src/mdstock_collector.o: src/mdstock_collector.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mdstock_collector.o src/mdstock_collector.cpp

${OBJECTDIR}/src/thread.o: src/thread.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread.o src/thread.cpp

${OBJECTDIR}/src/timer.o: src/timer.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/timer.o src/timer.cpp

${OBJECTDIR}/src/util.o: src/util.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/util.o src/util.cpp

${OBJECTDIR}/src/zmq_poller_reactor.o: src/zmq_poller_reactor.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/zmq_poller_reactor.o src/zmq_poller_reactor.cpp

${OBJECTDIR}/stcode_updator.o: stcode_updator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stcode_updator.o stcode_updator.cpp

${OBJECTDIR}/update_thread.o: update_thread.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DLINUX -DSPDLOG_TRACE_ON -DZMQ_BUILD_DRAFT_API -Iconf -Isrc -Ipublic/include -Ipublic/src -I../../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/update_thread.o update_thread.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libzmq.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stockmarket_v2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
