#ifndef PCOM_IPC_POSIX_CIRCULAR_H
#define PCOM_IPC_POSIX_CIRCULAR_H

#include <string>
#include <boost/asio/buffer.hpp>
#include "pcom/pcom_types.h"

using namespace boost::interprocess;


namespace pcom{

  class CircularBuffer{

  private:

    shared_memory_buffer<Alloc<unsigned char>>* data;

  public:

    CircularBuffer(){}

    CircularBuffer(shared_memory_buffer<Alloc<unsigned char>>* data):data(data){}

    bool push(const boost::asio::const_buffer& item){
        scoped_lock<interprocess_mutex> lock(data->mutex);
        if(next(data->write_) == data->read_){
          return false;
        }
        const unsigned char* head = boost::asio::buffer_cast<const unsigned char*>(item);
        std::copy(head, head + data->item_size_, data->cbuffer.begin() + data->write_);
        increment(&(data->write_));
        data->size_++;
        return true;
    }

    bool pop(boost::asio::mutable_buffer& item_dest){
      scoped_lock<interprocess_mutex> lock(data->mutex);
      if(data->read_ == data->write_){
        return false;
      }
      unsigned char* item_head = boost::asio::buffer_cast<unsigned char*>(item_dest);
      std::copy(data->cbuffer.begin()+data->read_, data->cbuffer.begin()+data->read_ + data->item_size_, item_head);
      increment(&(data->read_));
      data->size_--;
      return true;
    }

    const boost::asio::const_buffer operator [] (unsigned int i) const {
      scoped_lock<interprocess_mutex> lock(data->mutex);
      if(i >= data->size_){
        return boost::asio::const_buffer();
      }else{
        return boost::asio::const_buffer(&*(data->cbuffer.cbegin() + i*data->item_size_), data->item_size_);
      }
    }

    unsigned int size(){
      scoped_lock<interprocess_mutex> lock(data->mutex);
      return data->size_;
    }

  private:

    void increment(unsigned int* idx){
      *idx += data->item_size_;
      if(*idx >= data->cbuffer.size()){
        *idx=0;
      }
    }

    unsigned int next(unsigned int idx){
      idx += data->item_size_;
      if(idx >= data->cbuffer.size()){
        idx=0;
      }
      return idx;
    }

  };

}

#endif
