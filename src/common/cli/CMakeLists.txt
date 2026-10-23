add_library(ann_cli STATIC
  benchmark_cli.cpp
  benchmark_cli.h
  cli_options.cpp
  cli_options.h
)
add_library(ann::cli ALIAS ann_cli)

target_link_libraries(ann_cli PUBLIC ann::common)
