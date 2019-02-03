#ifndef KCOM_TYPES_H_
#define KCOM_TYPES_H_

// Boost
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <string>
#include <cstdlib> //std::system
#include <functional>

namespace kcom{

  using namespace boost::interprocess;

  typedef allocator<double, managed_shared_memory::segment_manager>  ShmemAllocator;

  typedef vector<double, ShmemAllocator> DataVector;

  struct NameId{
    NameId(std::string name): name(name){};
    std::string name;
  };

  struct DataSize{
    DataSize(long size): size(size){};
    long size;
  };

  enum MEMORY_CONSTRUCTION_TYPE {CREATE, READ};

  typedef std::function<void(DataVector*)> FuncDataTransfer;


}

#endif
