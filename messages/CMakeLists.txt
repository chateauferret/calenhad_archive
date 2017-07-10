cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (messages)
include (CMakeFindDependencyMacro)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Xml)


set(CMAKE_AUTOMOC ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Xml_INCLUDE_DIRS})

SET(MESSAGES_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/QNotificationService.h
        ${CMAKE_CURRENT_LIST_DIR}/QNotificationStack.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QNotificationStack.h
        ${CMAKE_CURRENT_LIST_DIR}/QNotification.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QNotification.h
        ${CMAKE_CURRENT_LIST_DIR}/QProgressNotification.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QProgressNotification.h)


#set (RESOURCES_DIR resources)
#qt5_add_resources (RESOURCES ${RESOURCES_DIR}/resources.qrc)
#MESSAGE (STATUS "Resources dir " ${RESOURCES})
add_library (messages ${MESSAGES_SOURCE_FILES})


target_link_libraries (messages Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Xml Marble)