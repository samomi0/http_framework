#include "http_framework/listener.hpp"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>

#include "LumosLog.hpp"

namespace http_framework {
Listener::Listener(boost::asio::io_context& io_context, unsigned short port, Router router)
    : io_context_(io_context),
      acceptor_(io_context_, {boost::asio::ip::tcp::v4(), port}),
      router_(std::move(router)) {
  LumosLog::log(LumosLog::LogLevel::INFO, "Listener", "Listener initialized on port {}", port);
}

void Listener::start() {
  LumosLog::log(LumosLog::LogLevel::INFO, "Listener", "Starting listener");
  do_accept();
}

void Listener::stop() {
  if (acceptor_.is_open()) {
    acceptor_.close();
    LumosLog::log(LumosLog::LogLevel::INFO, "Listener", "Listener stopped");
  }
}

void Listener::do_accept() {
  acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (!ec) {
      LumosLog::log(LumosLog::LogLevel::DEBUG, "Listener", "Accepted connection from {}", socket.remote_endpoint().address().to_string());

      boost::asio::co_spawn(io_context_, 
        [socket = std::move(socket), router = router_]() mutable -> boost::asio::awaitable<void> {
          try {
            auto req = co_await Request::parse(socket);
            auto res = router.handle_request(req);

            std::string response_str = res.to_string();
            co_await boost::asio::async_write(socket, boost::asio::buffer(response_str), boost::asio::use_awaitable);
          } catch (...) {
            LumosLog::log(LumosLog::LogLevel::ERROR, "Listener", "Request handling failed");
          } }, boost::asio::detached);
    } else {
      LumosLog::log(LumosLog::LogLevel::ERROR, "Listener", "Accept failed: {}", ec.message());
    }

    do_accept();
  });
}
}