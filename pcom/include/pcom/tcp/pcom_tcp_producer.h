#ifndef PCOM_TCP_PRODUCER_H
#define PCOM_TCP_PRODUCER_H

#include <thread>
#include "pcom/tcp/pcom_tcp_server.h"

namespace pcom{ namespace tcp{

class Producer{

public:

  Producer(unsigned int port):
  listen_endpoint(boost::asio::ip::tcp::v4(), port),
  broadcast_endpoint(boost::asio::ip::udp::v4(), port),
  server_(io_service_, listen_endpoint, broadcast_endpoint)
  {
    thread_ = std::thread(&Producer::run, this);
  }

  void send(const boost::asio::const_buffer& item){
    server_.channel_.deliver(item);
  }

private:

  void run(){io_service_.run();}

private:

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::endpoint listen_endpoint;
    boost::asio::ip::udp::endpoint broadcast_endpoint;
    pcom::server server_;
    std::thread thread_;

};

}}

#endif
