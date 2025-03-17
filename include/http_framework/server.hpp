#pragma once
#include <boost/asio.hpp>
#include <memory>
#include "router.hpp"
#include "listener.hpp"

namespace http_framework {
class Server {
public:
    void start(unsigned short port, Router router);
    void stop();

private:
    boost::asio::io_context io_context_;
    std::unique_ptr<Listener> listener_;
    Router router_;
};
}  // namespace http_framework