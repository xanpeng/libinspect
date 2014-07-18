#include <string>
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "message.h"
#include "mime_types.h"

namespace libinspect {

/// for Reply

namespace status_strings {
const std::string ok = "HTTP/1.0 200 OK\r\n";
const std::string created = "HTTP/1.0 201 Created\r\n";
const std::string accepted = "HTTP/1.0 202 Accepted\r\n";
const std::string no_content = "HTTP/1.0 204 No Content\r\n";
const std::string multiple_choices = "HTTP/1.0 300 Multiple Choices\r\n";
const std::string moved_permanently = "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily = "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified = "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request = "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized = "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden = "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found = "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error = "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented = "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway = "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable = "HTTP/1.0 503 Service Unavailable\r\n";

boost::asio::const_buffer ToBuffer(Reply::StatusType status) {
  switch (status) {
  case Reply::ok:
    return boost::asio::buffer(ok);
  case Reply::created:
    return boost::asio::buffer(created);
  case Reply::accepted:
    return boost::asio::buffer(accepted);
  case Reply::no_content:
    return boost::asio::buffer(no_content);
  case Reply::multiple_choices:
    return boost::asio::buffer(multiple_choices);
  case Reply::moved_permanently:
    return boost::asio::buffer(moved_permanently);
  case Reply::moved_temporarily:
    return boost::asio::buffer(moved_temporarily);
  case Reply::not_modified:
    return boost::asio::buffer(not_modified);
  case Reply::bad_request:
    return boost::asio::buffer(bad_request);
  case Reply::unauthorized:
    return boost::asio::buffer(unauthorized);
  case Reply::forbidden:
    return boost::asio::buffer(forbidden);
  case Reply::not_found:
    return boost::asio::buffer(not_found);
  case Reply::internal_server_error:
    return boost::asio::buffer(internal_server_error);
  case Reply::not_implemented:
    return boost::asio::buffer(not_implemented);
  case Reply::bad_gateway:
    return boost::asio::buffer(bad_gateway);
  case Reply::service_unavailable:
    return boost::asio::buffer(service_unavailable);
  default:
    return boost::asio::buffer(internal_server_error);
  }
}
} // namespace status_strings

namespace misc_strings {
const char name_value_seperator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };
} // namespace misc_strings

std::vector<boost::asio::const_buffer> Reply::ToBuffers() {
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(status_strings::ToBuffer(status));
  for (std::size_t i = 0; i < headers.size(); ++i) {
    Header& h = headers[i];
    buffers.push_back(boost::asio::buffer(h.name));
    buffers.push_back(boost::asio::buffer(misc_strings::name_value_seperator));
    buffers.push_back(boost::asio::buffer(h.value));
    buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  }
  buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}

namespace stock_replies {
const char ok[] = "";
const char created[] =
  "<html>"
  "<head><title>Created</title></head>"
  "<body><h1>201 Created</h1></body>"
  "</html>";
const char accepted[] =
  "<html>"
  "<head><title>Accepted</title></head>"
  "<body><h1>202 Accepted</h1></body>"
  "</html>";
const char no_content[] =
  "<html>"
  "<head><title>No Content</title></head>"
  "<body><h1>204 Content</h1></body>"
  "</html>";
const char multiple_choices[] =
  "<html>"
  "<head><title>Multiple Choices</title></head>"
  "<body><h1>300 Multiple Choices</h1></body>"
  "</html>";
const char moved_permanently[] =
  "<html>"
  "<head><title>Moved Permanently</title></head>"
  "<body><h1>301 Moved Permanently</h1></body>"
  "</html>";
const char moved_temporarily[] =
  "<html>"
  "<head><title>Moved Temporarily</title></head>"
  "<body><h1>302 Moved Temporarily</h1></body>"
  "</html>";
const char not_modified[] =
  "<html>"
  "<head><title>Not Modified</title></head>"
  "<body><h1>304 Not Modified</h1></body>"
  "</html>";
const char bad_request[] =
  "<html>"
  "<head><title>Bad Request</title></head>"
  "<body><h1>400 Bad Request</h1></body>"
  "</html>";
const char unauthorized[] =
  "<html>"
  "<head><title>Unauthorized</title></head>"
  "<body><h1>401 Unauthorized</h1></body>"
  "</html>";
const char forbidden[] =
  "<html>"
  "<head><title>Forbidden</title></head>"
  "<body><h1>403 Forbidden</h1></body>"
  "</html>";
const char not_found[] =
  "<html>"
  "<head><title>Not Found</title></head>"
  "<body><h1>404 Not Found</h1></body>"
  "</html>";
const char internal_server_error[] =
  "<html>"
  "<head><title>Internal Server Error</title></head>"
  "<body><h1>500 Internal Server Error</h1></body>"
  "</html>";
const char not_implemented[] =
  "<html>"
  "<head><title>Not Implemented</title></head>"
  "<body><h1>501 Not Implemented</h1></body>"
  "</html>";
const char bad_gateway[] =
  "<html>"
  "<head><title>Bad Gateway</title></head>"
  "<body><h1>502 Bad Gateway</h1></body>"
  "</html>";
const char service_unavailable[] =
  "<html>"
  "<head><title>Service Unavailable</title></head>"
  "<body><h1>503 Service Unavailable</h1></body>"
  "</html>";

std::string ToString(Reply::StatusType status) {
  switch (status) {
  case Reply::ok:
    return ok;
  case Reply::created:
    return created;
  case Reply::accepted:
    return accepted;
  case Reply::no_content:
    return no_content;
  case Reply::multiple_choices:
    return multiple_choices;
  case Reply::moved_permanently:
    return moved_permanently;
  case Reply::moved_temporarily:
    return moved_temporarily;
  case Reply::not_modified:
    return not_modified;
  case Reply::bad_request:
    return bad_request;
  case Reply::unauthorized:
    return unauthorized;
  case Reply::forbidden:
    return forbidden;
  case Reply::not_found:
    return not_found;
  case Reply::internal_server_error:
    return internal_server_error;
  case Reply::not_implemented:
    return not_implemented;
  case Reply::bad_gateway:
    return bad_gateway;
  case Reply::service_unavailable:
    return service_unavailable;
  default:
    return internal_server_error;
  }
}
} // namespace stock_replies

Reply Reply::StockReply(Reply::StatusType status) {
  Reply rep;
  rep.status = status;
  rep.content = stock_replies::ToString(status);
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/html";
  return rep;
}

/// RequestParser

RequestParser::RequestParser() : state_(method_start) {}

void RequestParser::Reset() {
  state_ = method_start;
}

boost::tribool RequestParser::Consume(Request& req, char input) {
  switch (state_) {
  case method_start:
    if (!IsChar(input) || IsCtl(input) || IsTspecial(input)) {
      return false;
    }
    else {
      state_ = method;
      req.method.push_back(input);
      return boost::indeterminate;
    }
  case method:
    if (input == ' ') {
      state_ = uri;
      return boost::indeterminate;
    }
    else if (!IsChar(input) || IsCtl(input) || IsTspecial(input)) {
      return false;
    }
    else {
      req.method.push_back(input);
      return boost::indeterminate;
    }
  case uri:
    if (input == ' ') {
      state_ = http_version_h;
      return boost::indeterminate;
    }
    else if (IsCtl(input)) {
      return false;
    }
    else {
      req.uri.push_back(input);
      return boost::indeterminate;
    }
  case http_version_h:
    if (input == 'H') {
      state_ = http_version_t_1;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_t_1:
    if (input == 'T') {
      state_ = http_version_t_2;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_t_2:
    if (input == 'T') {
      state_ = http_version_p;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_p:
    if (input == 'P') {
      state_ = http_version_slash;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_slash:
    if (input == '/') {
      req.http_version_major = 0;
      req.http_version_minor = 0;
      state_ = http_version_major_start;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_major_start:
    if (IsDigit(input)) {
      req.http_version_major = req.http_version_major * 10 + input - '0';
      state_ = http_version_major;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_major:
    if (input == '.') {
      state_ = http_version_minor_start;
      return boost::indeterminate;
    }
    else if (IsDigit(input)) {
      req.http_version_major = req.http_version_major * 10 + input - '0';
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_minor_start:
    if (IsDigit(input)) {
      req.http_version_minor = req.http_version_minor * 10 + input - '0';
      state_ = http_version_minor;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case http_version_minor:
    if (input == '\r') {
      state_ = expecting_newline_1;
      return boost::indeterminate;
    }
    else if (IsDigit(input)) {
      req.http_version_minor = req.http_version_minor * 10 + input - '0';
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case expecting_newline_1:
    if (input == '\n') {
      state_ = header_line_start;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case header_line_start:
    if (input == '\r') {
      state_ = expecting_newline_3;
      return boost::indeterminate;
    }
    else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
      state_ = header_lws;
      return boost::indeterminate;
    }
    else if (!IsChar(input) || IsCtl(input) || IsTspecial(input)) {
      return false;
    }
    else {
      req.headers.push_back(Header());
      req.headers.back().name.push_back(input);
      state_ = header_name;
      return boost::indeterminate;
    }
  case header_lws:
    if (input == '\r') {
      state_ = expecting_newline_2;
      return boost::indeterminate;
    }
    else if (input == ' ' || input == '\t') {
      return boost::indeterminate;
    }
    else if (IsCtl(input)) {
      return false;
    }
    else {
      state_ = header_value;
      req.headers.back().value.push_back(input);
      return boost::indeterminate;
    }
  case header_name:
    if (input == ':') {
      state_ = space_before_header_value;
      return boost::indeterminate;
    }
    else if (!IsChar(input) || IsCtl(input) || IsTspecial(input)) {
      return false;
    }
    else {
      req.headers.back().name.push_back(input);
      return boost::indeterminate;
    }
  case space_before_header_value:
    if (input == ' ') {
      state_ = header_value;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case header_value:
    if (input == '\r') {
      state_ = expecting_newline_2;
      return boost::indeterminate;
    }
    else if (IsCtl(input)) {
      return false;
    }
    else {
      req.headers.back().value.push_back(input);
      return boost::indeterminate;
    }
  case expecting_newline_2:
    if (input == '\n') {
      state_ = header_line_start;
      return boost::indeterminate;
    }
    else {
      return false;
    }
  case expecting_newline_3:
    return (input == '\n');
  default:
    return false;
  }
}

bool RequestParser::IsChar(int c) {
  return c >= 0 && c <= 127;
}

bool RequestParser::IsCtl(int c) {
  return (c >= 0 && c <= 31) || (c == 127);
}

bool RequestParser::IsTspecial(int c) {
  switch (c) {
  case '(': case ')': case '<': case '>': case '@':
  case ',': case ';': case ':': case '\\': case '"':
  case '/': case '[': case ']': case '?': case '=':
  case '{': case '}': case ' ': case '\t':
    return true;
  default:
    return false;
  } 
}

bool RequestParser::IsDigit(int c) {
  return c >= '0' && c <= '9';
}

/// RequestHandler

bool RequestHandler::DecodeUrl(const std::string& in, std::string& out) {
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '%') {
      if (i + 3 <= in.size()) {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value) {
          out += static_cast<char>(value);
          i += 2;
        }
        else return false;
      }
      else return false;
    }
    else if (in[i] == '+')
      out += ' ';
    else
      out += in[i];
  }
  return true;
}

}
