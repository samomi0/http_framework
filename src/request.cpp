#include "http_framework/request.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>

#include "LumosLog.hpp"

namespace http_framework {
boost::asio::awaitable<Request> Request::parse(boost::asio::ip::tcp::socket& socket) {
  LumosLog::log(LumosLog::LogLevel::DEBUG, "Request", "start parsing request");

  boost::asio::streambuf buf;

  co_await boost::asio::async_read_until(socket, buf, "\r\n\r\n", boost::asio::use_awaitable);

  std::istream stream(&buf);
  Request req;

  std::string request_line;
  std::getline(stream, request_line);
  std::vector<std::string> parts;
  boost::split(parts, request_line, boost::is_any_of(" "));
  if (parts.size() >= 2) {
    req.method_ = parts[0];
    req.path_ = parts[1];
  }

  std::string header_line;
  while (std::getline(stream, header_line) && header_line != "\r") {
    auto colon_pos = header_line.find(':');
    if (colon_pos != std::string::npos) {
      std::string header_name = header_line.substr(0, colon_pos);
      std::string header_value = header_line.substr(colon_pos + 1);
      boost::algorithm::trim(header_value);
      req.headers_[header_name] = header_value;
    }
  }

  if (req.headers_.find("Content-Length") != req.headers_.end()) {
    std::size_t content_length = std::stoul(req.headers_["Content-Length"]);
    if (content_length > 0) {
      std::size_t remaining_bytes = content_length - buf.size();
      if (remaining_bytes > 0) {
        co_await boost::asio::async_read(socket, buf, boost::asio::transfer_exactly(remaining_bytes), boost::asio::use_awaitable);
      }

      std::ostringstream body_stream;
      body_stream << &buf;
      req.body_ = body_stream.str();
    }
    
    auto number_of_characters_to_record = 32;
    if (number_of_characters_to_record > req.body().size()) {
      number_of_characters_to_record = req.body().size();
    }
    if (number_of_characters_to_record > 0) {
      std::string printable_message;
      for (int i = 0; i < number_of_characters_to_record; i++) {
        char ch = req.body()[i];
        if (ch < ' ' || ch > '~') {
          printable_message.push_back('.');
        } else {
          printable_message.push_back(ch);
        }
      }
      LumosLog::log(LumosLog::LogLevel::DEBUG, "Request", "The first {} characters:'{}'", number_of_characters_to_record, printable_message);
    }
  }

  co_return req;
}

void Request::add_param(const std::string& key, const std::string& value)  {
  query_params_[key] = value;
}

}  // namespace http_framework