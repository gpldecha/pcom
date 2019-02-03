#ifndef KCOM_SENSOR_PRODUCER_H_
#define KCOM_SENSOR_PRODUCER_H_

#include "kcom/kcom_types.h"

namespace kcom{

using namespace boost::interprocess;


class SharedMemory{

public:

  SharedMemory(NameId name_id, FuncDataTransfer& func_data_transfer);

  void initialise(DataSize data_size);

  void initialise();

  bool try_write();

  bool try_write(std::size_t num_tries);

  void write(bool &write);

  void read(bool &interrupt);

  bool try_read();

  bool try_read(int num_tries);


  //

  bool is_empty();

  void set_empty();

  void set_full();

private:

  NameId name_id;
  managed_shared_memory shm;
  FuncDataTransfer& func_data_transfer;
  DataVector* data;

};

}

#endif
