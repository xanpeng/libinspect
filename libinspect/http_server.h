#ifndef LIBINSPECT_SERVER_H
#define LIBINSPECT_SERVER_H

#include <string>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "connection.h"
#include "message.h"

namespace libinspect {

class HttpServer : private boost::noncopyable {
public:
  explicit HttpServer(const std::string& port);

  void Run();

  void RegisterRequestHandler(const RequestHandler::RealHandler& real_handler) {
    request_handler_.RegisterRealHandler(real_handler);
  }
private:
  void StartAccept();
  void HandleAccept(const boost::system::error_code& e);
  void HandleStop(); // handle a request to stop the server

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::signal_set signals_;
  ConnectionManager connection_manager_;
  ConnectionPtr new_connection_;
  RequestHandler request_handler_;
};

}

#endif

