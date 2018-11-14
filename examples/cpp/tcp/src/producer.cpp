#include <thread>
#include "pcom/tcp/pcom_tcp_producer.h"

int main(int argc, char** argv)
{

  pcom::tcp::Producer producer(6066);

  std::string msg;
  int count=0;
  while(true)
  {
    msg = std::to_string(count);
    producer.send(msg);
    count++;
    std::this_thread::sleep_for(std::chrono::duration<double>(1));
  }

  return 0;
}
