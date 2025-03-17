add_executable(demo_server demo_server.cpp)
target_link_libraries(demo_server http_framework spdlog::spdlog)