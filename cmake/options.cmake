set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message(FATAL_ERROR "ann-bench requires Clang; configure with -DCMAKE_CXX_COMPILER=clang++")
endif()

add_compile_options(
  -stdlib=libc++
  -Wall
  -Wextra
  -Wpedantic
  -fno-fast-math
  -ffp-contract=off
)
add_link_options(-stdlib=libc++)
