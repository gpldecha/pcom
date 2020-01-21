#ifndef PCAM_TYPES_H_
#define PCAM_TYPES_H_

#include <string>

#include "pcom/tcp/pcom_tcp_producer.h"
#include "pcom/tcp/pcom_tcp_consumer.h"


namespace pcam{

class VideoPublisher{
public:
    virtual void send(const boost::asio::const_buffer& item) = 0;
};

class VideoSubscriber{
public:
    virtual bool receive(std::vector<unsigned char>& msg) = 0;
};

class TCPVideoPublisher: public VideoPublisher{
public:
  TCPVideoPublisher(unsigned int port):
    producer(port)
    {}
    void send(const boost::asio::const_buffer& item){
      producer.send(item);
    }
private:
  pcom::tcp::Producer producer;
};

class TCPVideoSubscriber: public VideoSubscriber{
public:
    TCPVideoSubscriber(std::string host, unsigned int port, unsigned int buffer_size=5):
    consumer(host, port, buffer_size){}

    bool receive(std::vector<unsigned char>& msg){
        return consumer.receive(msg);
    }

private:
    pcom::tcp::Consumer consumer;
};



}

#endif
