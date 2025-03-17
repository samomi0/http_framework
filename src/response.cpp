#include "http_framework/response.hpp"
#include <sstream>

namespace http_framework {
  Response& Response::status(unsigned code) {
    status_code_ = code;
    return *this;
  }

  Response& Response::header(std::string key, std::string value) {
    headers_[std::move(key)] = std::move(value);
    return *this;
  }

  std::string Response::to_string() const {
    std::stringstream ss;
    ss << "HTTP/1.1 " << status_code_ << " OK\r\n";

    for (const auto& [key, val] : headers_) {
      ss << key << ": " << val << "\r\n";
    }

    ss << "Content-Length: " << body_.size() << "\r\n\r\n"
       << body_;

    return ss.str();
  }
}  // namespace http_framework