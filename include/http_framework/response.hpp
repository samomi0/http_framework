#pragma once
#include <string>
#include <unordered_map>

namespace http_framework {
class Response {
 public:
  Response() = default;

  /* 状态设置 */
  Response& status(unsigned code);

  /* 头操作 */
  Response& header(std::string key, std::string value);

  /* 正文设置 */
  template <typename T>
  Response& body(T&& content) {
    body_ = std::forward<T>(content);
    return *this;
  }

  /* 序列化为HTTP响应 */
  std::string to_string() const;

 private:
  unsigned status_code_ = 200;
  std::unordered_map<std::string, std::string> headers_;
  std::string body_;
};
}  // namespace http_framework