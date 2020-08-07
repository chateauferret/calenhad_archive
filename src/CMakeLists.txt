cmake_minimum_required(VERSION 3.6)
set (CUDA_PROPAGATE_HOST_FLAGS OFF)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
include (CMakeFindDependencyMacro)


set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
project (calenhad LANGUAGES CXX)
message (STATUS ("CMake prefix ") ${CMAKE_PREFIX_PATH})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Xml OpenGL)
find_package(Qwt REQUIRED)
find_package(GeographicLib 1.34 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

message (STATUS "QtCore v" ${Qt5Core_VERSION})
message (STATUS "QtWidgets v" ${Qt5Widgets_VERSION})
message (STATUS "QtGui v" ${Qt5Gui_VERSION})
message (STATUS "QtXml v" ${Qt5Xml_VERSION})
message (STATUS "QtOpenGL v" ${Qt5OpenGL_VERSION})
set (MESSAGES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/messages)
set (QMODULE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/module)
set (NODEEDIT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nodeedit)
set (CONTROLS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/controls)
set (PIPELINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pipeline)
set (ACTIONS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/actions)
set (MAPPING_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mapping)
set (LIBNOISEUTILS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libnoiseutils)
set (ICOSPHERE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/grid)
set (EXPRTK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/exprtk)
set (LEGEND_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/legend)
set (GRAPH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/graph)
set (PREFERENCES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/preferences)
set (COMPUTE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/compute)

include_directories(
        ${GeographicLib_INCLUDE_DIRS}
        ${QtWebApp_INCLUDE_DIRS}
        ..
        ${CMAKE_CURRENT_BINARY_DIR}
)

include (${MESSAGES_SOURCE_DIR}/CMakeLists.txt)
include (${QMODULE_SOURCE_DIR}/CMakeLists.txt)
include (${NODEEDIT_SOURCE_DIR}/CMakeLists.txt)
include (${CONTROLS_SOURCE_DIR}/CMakeLists.txt)
include (${PIPELINE_SOURCE_DIR}/CMakeLists.txt)
include (${ACTIONS_SOURCE_DIR}/CMakeLists.txt)
include (${MAPPING_SOURCE_DIR}/CMakeLists.txt)
include (${LIBNOISEUTILS_SOURCE_DIR}/CMakeLists.txt)
include (${ICOSPHERE_SOURCE_DIR}/CMakeLists.txt)
include (${EXPRTK_SOURCE_DIR}/CMakeLists.txt)
include (${LEGEND_SOURCE_DIR}/CMakeLists.txt)
include (${GRAPH_SOURCE_DIR}/CMakeLists.txt)
include (${PREFERENCES_SOURCE_DIR}/CMakeLists.txt)


set (CMAKE_AUTOMOC ON)
include_directories (..)
include_directories (${Qt5Widgets_INCLUDE_DIRS})
include_directories (${Qt5Core_INCLUDE_DIRS})
include_directories (${Qt5Xml_INCLUDE_DIRS})
include_directories (messages)

set (SOURCE_FILES ${SOURCE_FILES}
        geoutils.h
        geoutils.cpp
        CalenhadServices.cpp
        CalenhadServices.h
        Serializable.h
        matrices.cpp
        matrices.h
        noiseconstants.h
        CalenhadUi.cpp CalenhadUi.h
        ${MESSAGES_SOURCE_FILES}
        ${QMODULE_SOURCE_FILES}
        ${NODEEDIT_SOURCE_FILES}
        ${CONTROLS_SOURCE_FILES}
        ${PIPELINE_SOURCE_FILES}
        ${HTTPSERVER_SOURCE_FILES}
        ${ACTIONS_SOURCE_FILES}
        ${MAPPING_SOURCE_FILES}
        ${LIBNOISEUTILS_SOURCE_FILES}
        ${ICOSPHERE_SOURCE_FILES}
        ${EXPRTK_SOURCE_FILES}
        ${LEGEND_SOURCE_FILES}
        ${GRAPH_SOURCE_FILES}
        ${PREFERENCES_SOURCE_FILES})

set (RESOURCES_DIR resources)
qt5_add_resources (RESOURCES ${RESOURCES_DIR}/resources.qrc)

add_executable (calenhad main.cpp ${SOURCE_FILES} ${RESOURCES})
add_definitions (${GeographicLib_DEFINITIONS})
target_link_libraries (calenhad Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Xml Qt5::OpenGL
        ${GeographicLib_LIBRARIES}
        ${QtWebApp_LIBRARIES}
        ${PROJ4_LIBRARY}
        ${QWT_LIBRARY}
        ${OPENGL_LIBRARIES}
        )


