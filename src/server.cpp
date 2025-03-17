#include "http_framework/server.hpp"


#include <spdlog/spdlog.h>

namespace http_framework {

void Server::start(unsigned short port, Router router) {
  spdlog::info("Starting server");
  listener_ = std::make_unique<Listener>(io_context_, port, router_);
  listener_->start();
  io_context_.run();
}

void Server::stop() {
  if (listener_) {
    listener_->stop();
  }
  io_context_.stop();
  spdlog::info("Server stopped");
}
}  // namespace http_framework