#include "http_framework/router.hpp"

#include <regex>

namespace http_framework {

std::pair<std::regex, std::vector<std::string>> Router::parse_path_pattern(const std::string& pattern) {
  std::regex param_regex{R"(\{(\w+)\})"};
  std::sregex_iterator it(pattern.begin(), pattern.end(), param_regex);

  std::string regex_str = std::regex_replace(pattern, param_regex, R"((\w+))");
  regex_str = "^" + regex_str + "$";

  for (; it != std::sregex_iterator(); ++it) {
    param_names.push_back((*it)[1]);
  }

  return {std::regex(regex_str), param_names};
}

void Router::get(std::string path, Handler handler) {
  routes_.push_back({"GET", std::move(path), std::move(handler)});
}

void Router::post(std::string path, Handler handler) {
  routes_.push_back({"POST", std::move(path), std::move(handler)});
}

Response Router::handle_request(Request& req) {
  Response res;

  for (auto& middleware : middlewares_) {
    middleware(req, res);
  }

  for (const auto& route : routes_) {
    if (route.method != req.method()) continue;

    auto [pattern, param_names] = parse_path_pattern(route.path_pattern);
    std::smatch matches;

    if (std::regex_match(req.path(), matches, pattern)) {
      for (size_t i = 0; i < param_names.size(); ++i) {
        req.add_param(param_names[i], matches[i + 1].str());
      }
      return route.handler(req);
    }
  }

  return Response{}.status(404).body("Not Found");
}
}  // namespace http_framework