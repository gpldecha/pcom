#ifndef PCOM_CONSUMER_H
#define PCOM_CONSUMER_H

#include "pcom/pcom_types.h"
#include "pcom/pcom_circular_buffer.h"

namespace pcom{

  using namespace boost::interprocess;

  class Consumer{

  private:

    managed_shared_memory shm;
    shared_memory_buffer< Alloc<unsigned char> >* data;
    CircularBuffer circular_buffer;

  public:

    Consumer(NameId name_id)
    {
      shm = managed_shared_memory(open_only, name_id.name.c_str());
      data = shm.find<shared_memory_buffer<Alloc<unsigned char>>>("data").first;
      circular_buffer = CircularBuffer(data);
      if(data == 0){
        std::cerr<< "unable to find shared memory object" << std::endl;
      }
    }

    bool receive(boost::asio::mutable_buffer& item){
      return circular_buffer.pop(item);
    }

};

}

#endif
