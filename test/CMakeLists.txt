
cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project (CalenhadTest)
include (CMakeFindDependencyMacro)
enable_testing()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Xml Test)

set (test_SRCS
   #     geoutilstest.cpp
        LegendTest.cpp
        LegendTest.h
        ProjectionTest.cpp
        ProjectionTest.h
   #     IcosphereTest.cpp
   #     IcosphereTest.h
        QTestSuite.cpp
        QTestSuite.h
        test.cpp
        test.h)

add_executable(CalenhadTest ${test_SRCS})
add_test(CalenhadTest
        LegendTest
        IcosphereTest)

target_link_libraries(CalenhadTest Qt5::Test Qt5::Core Qt5::Widgets Qt5::Gui core)
