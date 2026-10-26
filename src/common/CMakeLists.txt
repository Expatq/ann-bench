add_library(ann_common STATIC
  backend.h
  backend_factory.h
  matrix.h
  metrics.h
  types.h
)
add_library(ann::common ALIAS ann_common)

target_include_directories(ann_common PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(ann_common PUBLIC cxx_std_23)
target_link_libraries(ann_common PUBLIC nlohmann_json::nlohmann_json)

add_subdirectory(config)
add_subdirectory(cli)
