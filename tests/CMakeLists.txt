find_package(Catch2 3 REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

include(CTest)
include(Catch)

add_subdirectory(unit)
add_subdirectory(integration)
