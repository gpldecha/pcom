#ifndef PCAM_TYPES_H_
#define PCAM_TYPES_H_

#include <string>

namespace pcam{

enum OUTPUT_TYPE{TCP, IPC};

class StreamMedium{
public:
  StreamMedium(OUTPUT_TYPE output_type):output_type_(output_type){}
  OUTPUT_TYPE output_type_;
  std::string hostname_;
  int port_;
};

}

#endif
