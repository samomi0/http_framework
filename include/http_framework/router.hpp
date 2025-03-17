#pragma once
#include <functional>
#include <regex>
#include <vector>

#include "request.hpp"
#include "response.hpp"

namespace http_framework {
using Handler = std::function<Response(const Request&)>;

class Router {
 public:
  
  /* 路由注册方法 */
  void get(std::string path, Handler handler);
  void post(std::string path, Handler handler);
  // void put(std::string path, Handler handler);
  // void del(std::string path, Handler handler);

  /* 中间件支持 */
  void use(std::function<void(Request&, Response&)> middleware);

  /* 路由匹配 */
  Response handle_request(Request& req);

  /* 路径参数解析 */
  std::pair<std::regex, std::vector<std::string>> parse_path_pattern(const std::string& pattern);

 private:
  struct Route {
    std::string method;
    std::string path_pattern;
    Handler handler;
  };

  std::vector<Route> routes_;
  std::vector<std::string> param_names;

  std::vector<std::function<void(Request&, Response&)>> middlewares_;
};
}  // namespace http_framework