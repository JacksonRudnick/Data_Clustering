file(GLOB_RECURSE LIB_SOURCES
    algo/*.cc
    data/*.cc
    util/*.cc

    external_validation/external_val.cc
    validation/validate.cc
)

add_library(clustering_lib ${LIB_SOURCES})

target_include_directories(clustering_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(data_clustering main.cc)
target_link_libraries(data_clustering clustering_lib)

add_executable(run_validation validation/main.cc)
target_link_libraries(run_validation clustering_lib)

add_executable(run_external_validation external_validation/main.cc)
target_link_libraries(run_external_validation clustering_lib)