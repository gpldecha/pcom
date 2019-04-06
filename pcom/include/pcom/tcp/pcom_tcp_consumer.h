#ifndef PCOM_TCP_CONSUMER_H
#define PCOM_TCP_CONSUMER_H

#include <thread>
#include "pcom/tcp/pcom_tcp_client.h"
#include <boost/circular_buffer.hpp>

namespace pcom{ namespace tcp{

class Consumer{

public:

  Consumer():
  msg_buffer_(5), client_(io_service_){
    
  }

  Consumer(std::string host, unsigned int port, unsigned int buffer_size=5):
  msg_buffer_(buffer_size), client_(io_service_, host, port, &msg_buffer_)
  {
       thread_ = std::thread(&Consumer::run, this);
  }

  bool receive(std::string& msg){
      if(msg_buffer_.empty()){
        return false;
      }
      msg = msg_buffer_.front();
      msg_buffer_.pop_front();
      return true;
  }

  bool is_alive(){
      return client_.is_alive();
  }

private:

  void run(){io_service_.run();}

private:

  boost::circular_buffer<std::string> msg_buffer_;
  boost::asio::io_service io_service_;
  pcom::client client_;
  std::thread thread_;

};

}}

#endif
