add_executable(example1 example1.cpp)
target_link_libraries(example1 PRIVATE ${PROJECT_NAME} opentelemetry-cpp::ostream_metrics_exporter)
