#ifndef PCOM_PRODUCER_H
#define PCOM_PRODUCER_H

#include "pcom/ipc/pcom_types.h"
#include "pcom/ipc/pcom_circular_buffer.h"

namespace pcom{

  using namespace boost::interprocess;

  class Producer{

  private:

    managed_shared_memory shm;
    shared_memory_buffer< Alloc<unsigned char> >* data;
    CircularBuffer circular_buffer;

  public:

    Producer(NameId name_id, NumItems num_items, ItemSize item_size)
    {
      shared_memory_object::remove(name_id.name.c_str());
      shm = managed_shared_memory(create_only, name_id.name.c_str(), 65536);
      Alloc<unsigned char> alloc(shm.get_segment_manager());
      data = shm.find_or_construct<shared_memory_buffer<Alloc<unsigned char>>>("data")(num_items, item_size, shm.get_segment_manager());
      circular_buffer = CircularBuffer(data);
    }

    bool send(const boost::asio::const_buffer& item){
      return circular_buffer.push(item);
    }
  };

}

#endif /* PCOM_PRODUCER_H */
