#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "pcom/tcp/pcom_tcp_producer.h"
#include "pcom/tcp/pcom_tcp_consumer.h"

using namespace boost::python;


class TCPProducer{
public:
  TCPProducer(){};
  TCPProducer(unsigned int port){
    producer.reset(new pcom::tcp::Producer(port));
  }
  std::shared_ptr<pcom::tcp::Producer> producer;
};


class TCPConsumer{
public:
  TCPConsumer(){};
  TCPConsumer(std::string host, unsigned int port, unsigned int buffer_size)
  {
    consumer.reset(new pcom::tcp::Consumer(host, port, buffer_size));
  }
  std::shared_ptr<pcom::tcp::Consumer> consumer;
  std::string msg;
};

void producer_send(TCPProducer& tcp_producer, std::string msg){
    tcp_producer.producer->send(msg);
}

boost::python::object consumer_receive(TCPConsumer& tcp_consumer){
  if(tcp_consumer.consumer->receive(tcp_consumer.msg)){
    return object(tcp_consumer.msg);
  }else{
    return object(); // None
  }
}

BOOST_PYTHON_MODULE(pypcomtcp){

 class_<TCPProducer>("TCPProducer", init<unsigned int>())
  .def("send", producer_send);

  class_<TCPConsumer>("TCPConsumer", init<std::string, unsigned int, unsigned int>())
   .def("receive", consumer_receive);

}
