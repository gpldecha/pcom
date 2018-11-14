#ifndef PCOM_CIRCULAR_cbufferH
#define PCOM_CIRCULAR_cbufferH

#include <boost/asio/buffer.hpp>
#include <vector>
#include <iostream>

void print_buffer(boost::asio::const_buffer const& buffer)
{
    auto first = boost::asio::buffer_cast<const char*>(buffer);
    auto last = first + boost::asio::buffer_size(buffer);

    std::copy(first, last, std::ostream_iterator<char>(std::cout));
    std::cout << std::endl;
}


namespace pcom{

class CircularBuffer{

public:

  struct QueueSize{
    QueueSize(unsigned int size):size_(size){};
    unsigned int size_;
  };

  struct ItemSizeBytes{
      ItemSizeBytes(unsigned int size): size_(size){};
      unsigned int size_;
  };

private:

  unsigned int queue_size_;
  std::size_t item_size_;
  unsigned int write_, read_;
  unsigned int num_items_;
  std::vector<unsigned char> cbuffer;
  unsigned int cbuffer_size;

public:

  CircularBuffer(QueueSize q_size, ItemSizeBytes b_size):
    queue_size_(q_size.size_), item_size_(b_size.size_), write_(0), read_(0), num_items_(0)
  {
      cbuffer_size = queue_size_*item_size_;
      cbuffer.resize(cbuffer_size);
  }

  void push(const boost::asio::const_buffer& item){
      const unsigned char* head = boost::asio::buffer_cast<const unsigned char*>(item);
      std::copy(head, head + item_size_, cbuffer.begin()+write_);
      increment(&write_);
      num_items_++;
  }

  void pop(boost::asio::mutable_buffer& item_dest){
    if(num_items_ == 0)
      return;
    unsigned char* item_head = boost::asio::buffer_cast<unsigned char*>(item_dest);
    std::copy(cbuffer.begin()+read_, cbuffer.begin()+read_ + item_size_, item_head);
    increment(&read_);
    num_items_--;
  }

  const boost::asio::const_buffer operator [] (unsigned int i) const {
    if(i >= num_items_){
      return boost::asio::const_buffer();
    }else{
      return boost::asio::const_buffer(&*(cbuffer.cbegin() + i*item_size_), item_size_);
    }
  }

  std::size_t size(){
    return num_items_;
  }

private:

  void increment(unsigned int* idx){
    *idx += item_size_;
    if(*idx >= cbuffer_size){
      *idx=0;
    }
  }

};

}

#endif
