cmake_minimum_required(VERSION 3.22)
project(QClientApp)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_AUTOMOC ON)

include(cmake/build.cmake)

qt_add_executable(QClientApp main.cpp)
target_link_libraries(QClientApp PRIVATE Qt::Core Qt::Quick)
