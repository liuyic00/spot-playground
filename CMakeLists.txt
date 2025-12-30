cmake_minimum_required(VERSION 3.10)
project(hello_spot CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories(/usr/local/lib)
include_directories(/usr/local/include)

set(MY_TARGETS
  hello
  constructFormula
  transform
)

foreach(tgt ${MY_TARGETS})
  add_executable(${tgt} src/${tgt}.cpp)
  target_link_libraries(${tgt} PRIVATE spot)
endforeach()