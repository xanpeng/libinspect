#ifndef LIBINSPECT_REQUEST_H
#define LIBINSPECT_REQUEST_H

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>

namespace libinspect {

struct Header {
  std::string name;
  std::string value;
};

struct Request {
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<Header> headers;
};

struct Reply {
  enum StatusType {
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    multiple_choices = 300,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503
  } status;

  std::vector<Header> headers;
  std::string content;

  std::vector<boost::asio::const_buffer> ToBuffers();
  static Reply StockReply(StatusType status);
};

class RequestHandler : private boost::noncopyable {
public:
  typedef boost::function<void (const Request&, Reply&)> RealHandler;

  // At the first, request handler is built in http server (code is inherited
  // from boost asio http server example).
  // Now we have to let the outside handle http request, so here it is.
  void HandleReqeust(const Request& req, Reply& rep) {
    handler_(req, rep); 
  }
  void RegisterRealHandler(const RealHandler& handler) {
    handler_ = handler;
  }

  static bool DecodeUrl(const std::string& in, std::string& out);
private:
  RealHandler handler_;
};

class RequestParser {
public:
  RequestParser();
  void Reset();

  template <typename InputIterator>
  boost::tuple<boost::tribool, InputIterator> Parse(
      Request& req, InputIterator begin, InputIterator end) {
    while (begin != end) {
      boost::tribool result = Consume(req, *begin++);
      if (result || !result)
        return boost::make_tuple(result, begin);
    }
    boost::tribool result = boost::indeterminate;
    return boost::make_tuple(result, begin);
  }

private:
  // handle the next char
  boost::tribool Consume(Request& req, char input);
  static bool IsChar(int c);
  static bool IsCtl(int c);
  static bool IsTspecial(int c);
  static bool IsDigit(int c);

  enum state {
    method_start,
    method,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    expecting_newline_2,
    expecting_newline_3
  } state_;
};

}

#endif
