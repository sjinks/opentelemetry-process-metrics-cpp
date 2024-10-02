include(CMakeFindDependencyMacro)
find_dependency(opentelemetry-cpp COMPONENTS api)

if(NOT TARGET wwa_opentelemetry_process_metrics)
    get_filename_component(OTEL_PROCESS_METRICS_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    list(APPEND CMAKE_MODULE_PATH ${OTEL_PROCESS_METRICS_CMAKE_DIR})
    include("${OTEL_PROCESS_METRICS_CMAKE_DIR}/wwa_opentelemetry_process_metrics-target.cmake")

    add_library(wwa::opentelemetry::process_metrics ALIAS wwa_opentelemetry_process_metrics)
endif()
