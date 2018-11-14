#include <thread>
#include "pcom/tcp/pcom_tcp_consumer.h"

int main(int argc, char** argv)
{
  pcom::tcp::Consumer consumer("127.0.0.1", 6066);
  std::string msg;
  while(true)
  {
    if(consumer.receive(msg)){
      std::cout<< msg << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::duration<double>(1));
  }


  return 0;
}
