#include <string>

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "pcom/ipc/pcom_utils.h"
#include "pcom/ipc/pcom_types.h"
#include "pcom/ipc/pcom_producer.h"
#include "pcom/ipc/pcom_consumer.h"
using namespace boost::python;


template <class T>
boost::python::list toPythonList(std::vector<T>& vector) {
    typename std::vector<T>::iterator iter;
    boost::python::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return list;
  }

class ArrayProducer{
public:
  ArrayProducer(){}

  ArrayProducer(std::string name, int array_size, int queue_size)
  : buff(array_size){
     producer.reset(new pcom::Producer(pcom::NameId(name), pcom::NumItems(queue_size), pcom::ItemSize(pcom::get_size(buff))));
  }
  std::vector<double> buff;
  std::shared_ptr<pcom::Producer> producer;
};

class ArrayConsumer{
public:
  ArrayConsumer(){}
  ArrayConsumer(std::string name, int array_size):
    output(array_size), buff(boost::asio::buffer(output))
  {
    consumer.reset( new pcom::Consumer(pcom::NameId(name)));
  }

  std::shared_ptr<pcom::Consumer> consumer;
  std::vector<double> output;
  boost::asio::mutable_buffer buff;

};

void producer_send(ArrayProducer& array_producer, boost::python::list& ns){
  for (int i = 0; i < len(ns); ++i){
    array_producer.buff[i] = boost::python::extract<double>(ns[i]);
  }
  array_producer.producer->send(boost::asio::buffer(array_producer.buff));
}

boost::python::list comsumer_receive(ArrayConsumer& array_consumer){
  if(array_consumer.consumer->receive(array_consumer.buff)){
    return toPythonList<double>(array_consumer.output);
  }else{
    return boost::python::list();
  }
}

BOOST_PYTHON_MODULE(pypcomipc){

  class_<ArrayProducer>("ArrayDoubleProducer", init<std::string, int, int>())
    .def("send", producer_send);

 class_<ArrayConsumer>("ArrayDoubleConsumer", init<std::string, int>())
  .def("receive", comsumer_receive);

}
