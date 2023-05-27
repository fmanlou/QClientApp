include(cmake/utils.cmake)

find_package(Qt6 REQUIRED COMPONENTS Core Protobuf Grpc Quick)

set(QtLibs Qt::Core Qt::Grpc Qt::Protobuf Qt::Quick)

set(QML_MODULE_TARGET "cpp")

# SOURCE
file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/cpp/*.cpp)

# SOURCE Include
list_subdirectories(SOURCE_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/cpp/)

qt_add_qml_module(
  ${QML_MODULE_TARGET}
  OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/imports/Cpp
  VERSION
  1.0
  URI
  "Cpp"
  RESOURCE_PREFIX
  ${CMAKE_SOURCE_DIR}/imports/Cpp/
  SOURCES
  ${SRCS})

set(DATA_PROTO ${CMAKE_SOURCE_DIR}/lib/proto/data.proto)
set(SERVICE_PROTO ${CMAKE_SOURCE_DIR}/lib/proto/service.proto)

qt_add_protobuf(${QML_MODULE_TARGET} PROTO_FILES ${DATA_PROTO})

qt_add_grpc(${QML_MODULE_TARGET} CLIENT PROTO_FILES ${SERVICE_PROTO})

target_include_directories(${QML_MODULE_TARGET}
                           PRIVATE ${SOURCE_INCLUDE_DIRECTORY})

target_link_libraries(${QML_MODULE_TARGET} PRIVATE ${QtLibs})

# for all test
find_package(Catch2 3 REQUIRED)

include(CTest)
include(Catch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# Test Include
list_subdirectories(TEST_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

# integrate test
set(IntegrateTest "IntegrateTest")
file(GLOB_RECURSE INTEGRATE_TEST_SRCS
     ${CMAKE_SOURCE_DIR}/tests/cpp/integration/*.cpp)
add_executable(${IntegrateTest} ${INTEGRATE_TEST_SRCS})
target_link_libraries(${IntegrateTest} PRIVATE Catch2::Catch2WithMain ${QtLibs}
                                               ${QML_MODULE_TARGET})
target_include_directories(${IntegrateTest} PRIVATE ${SOURCE_INCLUDE_DIRECTORY}
                                                    ${TEST_INCLUDE_DIRECTORY})
catch_discover_tests(${IntegrateTest})

# unit test
set(UnitTest "UnitTest")
file(GLOB_RECURSE UNIT_TEST_SRCS ${CMAKE_SOURCE_DIR}/tests/cpp/unit/*.cpp)
add_executable(${UnitTest} ${UNIT_TEST_SRCS})
target_link_libraries(${UnitTest} PRIVATE Catch2::Catch2 ${QtLibs}
                                          ${QML_MODULE_TARGET})
target_include_directories(${UnitTest} PRIVATE ${SOURCE_INCLUDE_DIRECTORY}
                                               ${TEST_INCLUDE_DIRECTORY})
catch_discover_tests(${UnitTest})
