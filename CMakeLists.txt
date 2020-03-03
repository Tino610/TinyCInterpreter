cmake_minimum_required(VERSION 2.8.9)
project(TinyCInterpreter)

SET(CMAKE_VERBOSE_MAKEFILE "1")

IF (CMAKE_BUILD_TYPE STREQUAL "")
	MESSAGE("SET CMAKE_BUILD_TYPE=Release")
	set(CMAKE_BUILD_TYPE Release)
ELSE (CMAKE_BUILD_TYPE STREQUAL "")
	MESSAGE("CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})
ENDIF (CMAKE_BUILD_TYPE STREQUAL "")


SET (CMAKE_CXX_FLAGS "-Werror -std=c++11 -fvisibility=hidden -fno-omit-frame-pointer")

SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
SET(CMAKE_CXX_FLAGS_RELEASE "-g -O3 ")

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)

ADD_EXECUTABLE(main ${CMAKE_CURRENT_SOURCE_DIR}/TinyCInterpreter.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)
TARGET_LINK_LIBRARIES(main  tcc dl)
ADD_DEPENDENCIES(main build_tcc)


SET(TCC_PATH ./tcc-0.9.27)
add_custom_target(build_tcc ALL
	COMMAND bash -c \"cd ${CMAKE_CURRENT_SOURCE_DIR}/${TCC_PATH} && ./configure --disable-static && make\"
	COMMAND bash -c \"cp -r ${CMAKE_CURRENT_SOURCE_DIR}/${TCC_PATH}/{libtcc.so,include,libtcc1.a,libtcc.h} ${CMAKE_CURRENT_SOURCE_DIR}/.\"
	COMMENT  "build tcc source file and install")

