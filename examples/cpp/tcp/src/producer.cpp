#include <thread>
#include "pcom/tcp/pcom_tcp_producer.h"
#include <sched.h>

int main(int argc, char** argv)
{
  struct sched_param sched;
  sched.sched_priority = 1;
  if (sched_setscheduler(getpid(), SCHED_FIFO, &sched) < 0) {
      exit(EXIT_FAILURE);
    }

    std::cout<< "getpid(): " << getpid() << std::endl;

  pcom::tcp::Producer producer(6066);

  std::string msg;
  int count=0;
  while(true)
  {
    msg = std::to_string(count);
    //producer.send(boost::asio::buffer(msg));
    count++;
    std::this_thread::sleep_for(std::chrono::duration<double>(1));
  }

  return 0;
}
