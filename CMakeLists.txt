cmake_minimum_required(VERSION 3.2.3)

# boost directory
SET(BOOST_ROOT $ENV{BOOST_ROOT})
SET(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Boost 1.56.0 COMPONENTS system thread date_time timer chrono regex REQUIRED)

# to use unicode and avoid errors
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

# MFC
# ADD_DEFINITIONS(-D_AFXDLL)
# SET(CMAKE_MFC_FLAG 2)

SET(PROJ_NAME gw2dps)
PROJECT(${PROJ_NAME})


# gw2dps.dll
ADD_LIBRARY(${PROJ_NAME}_lib SHARED 
	gw2dps/config.h 
	gw2dps/config.cpp 
	gw2dps/keymap.h 
	gw2dps/keymap.cpp 
	gw2dps/hotkey.h 
	gw2dps/hotkey.cpp 
	gw2dps.cpp)

SET_TARGET_PROPERTIES(${PROJ_NAME}_lib PROPERTIES 
	FOLDER ${PROJ_NAME}_dll)

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}_lib PUBLIC 
	${Boost_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(${PROJ_NAME}_lib 
	hacklib_gw2 
	${Boost_LIBRARIES})


# gw2dps.exe
ADD_EXECUTABLE(${PROJ_NAME}_exe 
	gw2inject.cpp)

SET_TARGET_PROPERTIES(${PROJ_NAME}_exe PROPERTIES 
	FOLDER ${PROJ_NAME}_exe)

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}_exe PUBLIC 
	${Boost_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(${PROJ_NAME}_exe 
	${Boost_LIBRARIES})

IF(ARCH_64BIT)
    SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE EXCLUDE_FROM_DEFAULT_BUILD TRUE)
ENDIF()