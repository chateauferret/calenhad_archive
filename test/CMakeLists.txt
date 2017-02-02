cmake_minimum_required(VERSION 3.6)
include(CMakeFindDependencyMacro)
set (REPO ~/ClionProjects/.repo)

project (test)

include (CTest)
enable_testing()

#set(gtest_disable_pthreads on) #needed in MinGW
include (${REPO}/DownloadProject/DownloadProject.cmake)
download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

add_subdirectory (${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

set (test_SRCS
        geoutilstest.cpp
        icospheretest.cpp
        icospheretilesourcetest.cpp
        legendtest.cpp
        vertextest.cpp)

#qt5_use_modules (runUnitTests Gui Widgets OpenGL)
add_executable (runUnitTests ${test_SRCS})
target_link_libraries (runUnitTests gtest_main gmock core)
add_test (runUnitTests runUnitTests) #included in all tutorials but I don't know what it actually does.