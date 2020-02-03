cmake_minimum_required( VERSION 3.0 )

PROJECT( edgeDefect )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

include_directories(
${PROJECT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME}
src/main.cc
src/lsd.cc
)
target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
)
