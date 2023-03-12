cmake_minimum_required(VERSION 3.14)

project(NLog)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
	NLog.cpp
	NLog.h
	main.cpp
	NLog_define.h
)

add_executable(
	${PROJECT_NAME}
	${PROJECT_SOURCES}
)
