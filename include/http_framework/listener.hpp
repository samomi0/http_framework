#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "request.hpp"
#include "router.hpp"

namespace http_framework {
class Listener {
 public:
  Listener(boost::asio::io_context& io_context, unsigned short port, Router router);

  void start();
  void stop();

 private:
  void do_accept();

  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  Router router_;
};
}  // namespace http_framework