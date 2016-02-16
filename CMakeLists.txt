cmake_minimum_required(VERSION 3.2.3)

SET(PROJ_NAME gw2dps)
PROJECT(${PROJ_NAME})

#wxWidget directory
SET(wxWidgets_ROOT_DIR $ENV{WXWIDGET_ROOT})

FIND_PACKAGE(wxWidgets REQUIRED core base)

# boost directory
SET(BOOST_ROOT $ENV{BOOST_ROOT})
SET(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Boost 1.59.0 REQUIRED system thread date_time timer chrono regex)

# to use unicode and avoid errors
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

# MFC
# ADD_DEFINITIONS(-D_AFXDLL)
# SET(CMAKE_MFC_FLAG 2)


# gw2dps.dll
ADD_LIBRARY(${PROJ_NAME}_dll SHARED
	gw2dps/config.h
	gw2dps/config.cpp
	gw2dps/keymap.h
	gw2dps/keymap.cpp
	gw2dps/hotkey.h
	gw2dps/hotkey.cpp
	gw2dps/preferences.h
	gw2dps/preferences.cpp
	gw2dps/gw2dps.h
	gw2dps/gw2dps.cpp)

SET_TARGET_PROPERTIES(${PROJ_NAME}_dll PROPERTIES
	FOLDER ${PROJ_NAME}_dll
	OUTPUT_NAME ${PROJ_NAME})

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}_dll PUBLIC
	${Boost_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(${PROJ_NAME}_dll
	hacklib_gw2
	${Boost_LIBRARIES})


# gw2dps.exe
ADD_EXECUTABLE(${PROJ_NAME}_exe WIN32
	gw2dps/config.h
	gw2dps/config.cpp
	gw2dps/keymap.h
	gw2dps/keymap.cpp
	gw2dps/hotkey.h
	gw2dps/hotkey.cpp
	injector/MainFrame.h
	injector/MainFrame.cpp
	injector/ChangeKeyFrame.h
	injector/ChangeKeyFrame.cpp
	injector/MainPane.h
	injector/MainPane.cpp
	injector/MainApp.h
	injector/MainApp.cpp
	injector/Inject.h
	injector/Inject.cpp
	injector/HotKeyChangeButton.h
	injector/HotKeyChangeButton.cpp
	injector/Icon.rc)

SET_TARGET_PROPERTIES(${PROJ_NAME}_exe PROPERTIES
	FOLDER ${PROJ_NAME}_exe
	OUTPUT_NAME ${PROJ_NAME})

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}_exe PUBLIC
	${Boost_INCLUDE_DIR}
	${wxWidgets_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(${PROJ_NAME}_exe
	${Boost_LIBRARIES}
	${wxWidgets_LIBRARIES})

IF(ARCH_64BIT)
    SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE EXCLUDE_FROM_DEFAULT_BUILD TRUE)
ENDIF()