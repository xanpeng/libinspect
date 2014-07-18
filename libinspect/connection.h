#ifndef LIBINSPECT_CONNECTION_H
#define LIBINSPECT_CONNECTION_H 

#include <set>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "message.h"

namespace libinspect {

class ConnectionManager;

class Connection
  : public boost::enable_shared_from_this<Connection>,
    private boost::noncopyable {
public:
  explicit Connection(boost::asio::io_service& io_service,
      ConnectionManager& manager, RequestHandler& handler);

  void Start();
  void Stop();

  boost::asio::ip::tcp::socket& socket();
private:
  void HandleRead(const boost::system::error_code& e, std::size_t bytes_transfered);
  void HandleWrite(const boost::system::error_code& e);

  boost::asio::ip::tcp::socket socket_;
  boost::array<char, 8192> buffer_; // for incoming data
  Request request_;
  Reply reply_;
  RequestParser request_parser_;
  RequestHandler& request_handler_;

  ConnectionManager& connection_manager_;
};
typedef boost::shared_ptr<Connection> ConnectionPtr;

class ConnectionManager : private boost::noncopyable {
public:
  void Start(ConnectionPtr c);
  void Stop(ConnectionPtr c);
  void StopAll();
private:
  std::set<ConnectionPtr> connections_;
};

}

#endif
