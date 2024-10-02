set(public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/wwa/opentelemetry_process_metrics//export.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/wwa/opentelemetry_process_metrics/process_metrics.h
)

add_library(
    ${PROJECT_NAME}
    process_metrics.cpp
    observers/rusage.cpp
    observers/status.cpp
    observers/debouncing_observer.cpp
    observers/process_cpu_time.cpp
    observers/process_cpu_utilization.cpp
    observers/process_memory_usage.cpp
    observers/process_memory_virtual.cpp
    observers/process_disk_io.cpp
    observers/process_network_io.cpp
    observers/process_thread_count.cpp
    observers/process_open_fd_count.cpp
    observers/process_context_switches.cpp
    observers/process_paging_faults.cpp
    observers/utils.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON
        PUBLIC_HEADER "${public_headers}"
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE opentelemetry-cpp::api)

if(NOT BUILD_SHARED_LIBS)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -DWWA_OPENTELEMETRY_PROCESS_METRICS_STATIC_DEFINE)
endif()
