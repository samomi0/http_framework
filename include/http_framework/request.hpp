#pragma once
#include <boost/asio.hpp>
#include <string>
#include <unordered_map>

namespace http_framework {
class Request {
 public:
  using Headers = std::unordered_map<std::string, std::string>;
  using Params = std::unordered_map<std::string, std::string>;

  /* 从socket异步解析 */
  static boost::asio::awaitable<Request> parse(boost::asio::ip::tcp::socket& socket);

  const std::string& method() const { return method_; }
  const std::string& path() const { return path_; }
  const Headers& headers() const { return headers_; }
  const Params& query_params() const { return query_params_; }
  const std::string& body() const { return body_; }
  void add_param(const std::string& key, const std::string& value);

 private:
  std::string method_;
  std::string path_;
  Headers headers_;
  Params query_params_;
  std::string body_;
};
}  // namespace http_framework