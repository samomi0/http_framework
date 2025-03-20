#include "http_framework/router.hpp"
#include "http_framework/server.hpp"

#include "LogAdapter.hpp"

int main() {
  initLumosLog();

  http_framework::Router router;
  router.get("/hello", [](const auto& req) {
    return http_framework::Response{}
        .body("Hello World for GET")
        .header("Content-Type", "text/plain")
        .header("Strict-Transport-Security", "max-age=31536000; includeSubDomains")
        .header("Content-Security-Policy", "default-src 'self'; script-src 'self'; object-src 'none'")
        .header("X-Content-Type-Options", "nosniff")
        .header("X-Frame-Options", "DENY")
        .header("X-XSS-Protection", "1; mode=block")
        .header("Referrer-Policy", "no-referrer")
        .header("Permissions-Policy", "geolocation=()")
        .header("Expect-CT", "enforce, max-age=86400");
  });
  router.post("/hello", [](const auto& req) {
    return http_framework::Response{}
        .body("Hello World for POST")
        .header("Content-Type", "text/plain");
  });

  http_framework::Server server;
  server.start(8080, std::move(router));
  
  return 0;
}