#ifndef PCOM_TYPES_H
#define PCOM_TYPES_H

// STL
#include <string>

// Boost
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

namespace pcom{

struct NameId{
  NameId(std::string name): name(name){};
  std::string name;
};

struct NumItems{
  NumItems(unsigned int size):value(size){};
  unsigned int value;
};

struct ItemSize{
  ItemSize(unsigned int size): value(size){};
  unsigned int value;
};

using namespace boost::interprocess;

template <typename T>
using Alloc = allocator<T, managed_shared_memory::segment_manager>;

template <typename Alloc >
struct shared_memory_buffer{
  using allocator_type = typename Alloc::template rebind<unsigned char>::other;

  shared_memory_buffer(NumItems num_items, ItemSize item_size, Alloc alloc = {}):
  cbuffer(num_items.value*item_size.value, alloc)
  {
    num_items_ = num_items.value;
    item_size_ = item_size.value;
    write_ = 0;
    read_ = 0;
  }

  std::vector<unsigned char, Alloc> cbuffer;
  unsigned int num_items_;
  unsigned int item_size_;
  unsigned int write_, read_;
  boost::interprocess::interprocess_mutex mutex;
};


}

#endif
