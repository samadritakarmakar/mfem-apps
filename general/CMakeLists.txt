project(general)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("../mfem")
find_package(jsoncpp REQUIRED)

add_library(${PROJECT_NAME} jsonRootParser.hpp general.hpp)
target_link_libraries(${PROJECT_NAME} jsoncpp)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
