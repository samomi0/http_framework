set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} HTTP_FRAMEWORK_SOURCES)

function(configure_http_framework_target target)
  target_include_directories(${target} PUBLIC
    include
    ${Boost_INCLUDE_DIRS}
    ${fmt_INCLUDE_DIRS}
  )
  target_link_libraries(${target} PRIVATE
    Boost::boost
    Boost::system
    spdlog::spdlog
    fmt::fmt
  )
  target_compile_features(${target} PUBLIC cxx_std_20)
endfunction()

add_library(http_framework_objects OBJECT ${HTTP_FRAMEWORK_SOURCES})
configure_http_framework_target(http_framework_objects)

add_library(http_framework SHARED $<TARGET_OBJECTS:http_framework_objects>)
configure_http_framework_target(http_framework)

add_library(http_framework_static STATIC $<TARGET_OBJECTS:http_framework_objects>)
configure_http_framework_target(http_framework_static)
set_target_properties(http_framework_static PROPERTIES OUTPUT_NAME http_framework)