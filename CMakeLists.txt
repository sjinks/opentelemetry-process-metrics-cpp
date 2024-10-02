cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    wwa_opentelemetry_process_metrics
    VERSION 1.0.0
    DESCRIPTION "OpenTelemetry Process Metrics library"
    LANGUAGES CXX
)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(WITH_EXAMPLES "Whether to enable examples" ON)
option(INSTALL_PROCESS_METRICS "Whether to install Process Metrics library" ON)
option(MAINTAINER_MODE "Whether to enable maintainer mode" OFF)

find_package(opentelemetry-cpp)

if(MAINTAINER_MODE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wall -Wextra -Werror)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        add_compile_options(-Weverything -Wno-pre-c++17-compat -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors)
    endif()
endif()

add_subdirectory(src)

if(WITH_EXAMPLES)
    if(TARGET opentelemetry-cpp::ostream_metrics_exporter)
        add_subdirectory(examples)
    else()
        message(WARNING "opentelemetry-cpp::ostream_metrics_exporter not found, skipping examples")
    endif()
endif()

if(INSTALL_PROCESS_METRICS)
    install(TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}-target"
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/wwa/opentelemetry_process_metrics"
    )

    install(
        EXPORT "${PROJECT_NAME}-target"
        FILE "${PROJECT_NAME}-target.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    if(BUILD_SHARED_LIBS)
        set(REQUIRES_SUFFIX ".private")
    endif()
    configure_file("${CMAKE_SOURCE_DIR}/cmake/pkg-config.pc.in" "${PROJECT_NAME}.pc" @ONLY)
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    )
endif()

find_program(CLANG_FORMAT NAMES clang-format clang-format-19 clang-format-18 clang-format-17)
find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-19 clang-tidy-18 clang-tidy-17)

if(CLANG_FORMAT)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT} --Wno-error=unknown -i -style=file ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

if(CLANG_TIDY)
    add_custom_target(
        tidy
        COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
