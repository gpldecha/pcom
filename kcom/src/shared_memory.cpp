#include "kcom/shared_memory.h"
#include <iostream>
namespace kcom{

SharedMemory::SharedMemory(NameId name_id, FuncDataTransfer& func_data_transfer):
   name_id(name_id),
  func_data_transfer(func_data_transfer)
  {}

void SharedMemory::initialise(DataSize data_size){
  shared_memory_object::remove(name_id.name.c_str());
  shm = managed_shared_memory(create_only, name_id.name.c_str(), 4096);
  const ShmemAllocator alloc_inst(shm.get_segment_manager());
  data = shm.find_or_construct<DataVector>(name_id.name.c_str())(alloc_inst);
  for(int i = 0; i < data_size.size+1; ++i){
    data->push_back(0);
  }
}

void SharedMemory::initialise(){
    try{
        shm = managed_shared_memory(open_only, name_id.name.c_str());
        const ShmemAllocator alloc_inst(shm.get_segment_manager());
        data = shm.find_or_construct<DataVector>(name_id.name.c_str())(alloc_inst);
    } catch (const boost::interprocess::interprocess_exception& ex) {
       std::cerr << "boost::interprocess::interprocess_exception: " << ex.what()
                 << std::endl;
     } catch (const std::exception& ex) {
       std::cerr << "std::exception: " << ex.what() << std::endl;
     } catch (...) {
       std::cerr << "unhandled exception\n";
     }
}


// write

void SharedMemory::write(bool& interrupt){
    while(!is_empty() && interrupt){} // wait that it is empty before writing to it
    func_data_transfer(data);
    set_full();
}

bool SharedMemory::try_write(){
    if(is_empty()){
        func_data_transfer(data);
        set_full();
        return true;
    }
    return false;
}

bool SharedMemory::try_write(std::size_t num_tries){
     while(!is_empty() || num_tries >=0 ){
        num_tries--;
    }
    if(!is_empty())
        return false;
    func_data_transfer(data);
    set_full();
    return true;
}


// read

void SharedMemory::read(bool& interrupt){
    while(is_empty() && interrupt){}
    func_data_transfer(data);
    set_empty();
}

bool SharedMemory::try_read(){
    if(!is_empty()){
        func_data_transfer(data);
        set_empty();
        return true;
    }
    return false;
}

bool SharedMemory::try_read(int num_tries){
     while(is_empty() && num_tries >=0){
        num_tries--;
    }
    if(is_empty())
        return false;
    func_data_transfer(data);
    set_empty();
    return true;
}


bool SharedMemory::is_empty(){
    if((*data)[0] == 0){
        return true;
    }
    return false;
}

void SharedMemory::set_empty(){
    (*data)[0]=0;
}

void SharedMemory::set_full(){
    (*data)[0]=1;
}


}
