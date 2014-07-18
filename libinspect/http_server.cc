#include <signal.h>
#include <utility>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "http_server.h"

namespace libinspect {

HttpServer::HttpServer(const std::string& port)
  : io_service_(),
    acceptor_(io_service_),
    signals_(io_service_),
    connection_manager_(),
    new_connection_(),
    request_handler_() {
  signals_.add(SIGINT);
  signals_.add(SIGTERM);
  signals_.add(SIGQUIT);
  signals_.async_wait(boost::bind(&HttpServer::HandleStop, this));

  using boost::asio::ip::tcp;
  tcp::endpoint endpoint(tcp::v4(), boost::lexical_cast<int>(port));
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  StartAccept();
}

void HttpServer::Run() {
  io_service_.run();
}

void HttpServer::StartAccept() {
  new_connection_.reset(
      new Connection(io_service_, connection_manager_, request_handler_));
  acceptor_.async_accept(new_connection_->socket(),
      boost::bind(&HttpServer::HandleAccept, this, boost::asio::placeholders::error));
}

void HttpServer::HandleAccept(const boost::system::error_code& e) {
  if (!acceptor_.is_open())
    return;
  if (!e)
    connection_manager_.Start(new_connection_);
  StartAccept();
}

void HttpServer::HandleStop() {
  acceptor_.close();
  connection_manager_.StopAll();
}

}
