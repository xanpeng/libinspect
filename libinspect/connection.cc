#include <algorithm>
#include <vector>
#include <boost/bind.hpp>
#include "connection.h"
#include "message.h"

namespace libinspect {

Connection::Connection(boost::asio::io_service& io_service,
    ConnectionManager& manager, RequestHandler& handler)
  : socket_(io_service),
    request_handler_(handler),
    connection_manager_(manager) {}

boost::asio::ip::tcp::socket& Connection::socket() {
  return socket_;
}

void Connection::Start() {
  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&Connection::HandleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void Connection::Stop() {
  socket_.close();
}

void Connection::HandleRead(const boost::system::error_code& e, std::size_t bytes_transferred) {
  if (!e) {
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.Parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result) {
      request_handler_.HandleReqeust(request_, reply_);
      boost::asio::async_write(socket_, reply_.ToBuffers(),
          boost::bind(&Connection::HandleWrite, shared_from_this(), boost::asio::placeholders::error));
    }
    else if (!result) {
      reply_ = Reply::StockReply(Reply::bad_request);
      boost::asio::async_write(socket_, reply_.ToBuffers(),
          boost::bind(&Connection::HandleWrite, shared_from_this(), boost::asio::placeholders::error));
    }
    else {
      socket_.async_read_some(boost::asio::buffer(buffer_),
          boost::bind(&Connection::HandleRead, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
  }
  else if (e != boost::asio::error::operation_aborted) {
    connection_manager_.Stop(shared_from_this());
  }
}

void Connection::HandleWrite(const boost::system::error_code& e) {
  if (!e) {
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
  if (e != boost::asio::error::operation_aborted) {
    connection_manager_.Stop(shared_from_this());
  }
}

void ConnectionManager::Start(ConnectionPtr c) {
  connections_.insert(c);
  c->Start();
}

void ConnectionManager::Stop(ConnectionPtr c) {
  connections_.erase(c);
  c->Stop();
}

void ConnectionManager::StopAll() {
  std::for_each(connections_.begin(), connections_.end(),
      boost::bind(&Connection::Stop, _1));
  connections_.clear();
}

}
