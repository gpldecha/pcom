#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <signal.h>
#include "pcom/ipc/pcom_consumer.h"
#include "pcom/ipc/pcom_utils.h"

using namespace std;
using namespace pcom;

bool is_running = true;

void sighandler(int sig)
{
    is_running = false;
}

int main(int argc, char** argv){

  signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);

  Consumer consumer(NameId("my_data"));

  //std::array<int, 1> output;
  std::vector<double> output(1);
  boost::asio::mutable_buffer b = boost::asio::buffer(output);

  std::cout<< "Start consuming" << std::endl;
  while(is_running){
    if(consumer.receive(b)){
      std::cout<< "received: " << output[0] << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  std::cout<< "exit" << std::endl;
  return 0;
}
