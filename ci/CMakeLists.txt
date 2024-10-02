cmake_minimum_required(VERSION 3.25)

project(ci VERSION 1.0.0 LANGUAGES CXX)

include(FindPkgConfig)

find_package(wwa_opentelemetry_process_metrics CONFIG REQUIRED)
pkg_check_modules(wwa_opentelemetry_process_metrics REQUIRED IMPORTED_TARGET wwa_opentelemetry_process_metrics)

add_executable(ci-cmake ../examples/example1.cpp)
target_link_libraries(ci-cmake PRIVATE wwa::opentelemetry::process_metrics opentelemetry-cpp::ostream_metrics_exporter)
target_compile_definitions(ci-cmake PRIVATE -DUSE_SYSTEM_INCLUDE)

add_executable(ci-pkgconfig ../examples/example1.cpp)
target_link_libraries(ci-pkgconfig PRIVATE PkgConfig::wwa_opentelemetry_process_metrics opentelemetry-cpp::ostream_metrics_exporter)
target_compile_definitions(ci-pkgconfig PRIVATE -DUSE_SYSTEM_INCLUDE)
