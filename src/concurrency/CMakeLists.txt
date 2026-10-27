find_package(Threads REQUIRED)

add_library(ann_concurrency STATIC
  thread_pool.cpp
  thread_pool.h
)
add_library(ann::concurrency ALIAS ann_concurrency)

target_include_directories(ann_concurrency PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_compile_features(ann_concurrency PUBLIC cxx_std_23)
target_link_libraries(ann_concurrency PUBLIC ann::util Threads::Threads)
