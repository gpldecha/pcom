#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <signal.h>
#include "pcom/pcom_consumer.h"
#include "pcom/pcom_utils.h"

using namespace std;
using namespace pcom;

bool is_running = true;

void sighandler(int sig)
{
    cout<< "\nSignal " << sig << " caught..." << endl;
    is_running = false;
}

int main(int argc, char** argv){

  signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);

  Consumer consumer(NameId("array_data"));

  std::cout<< "Start consuming" << std::endl;

  std::array<double, 3> output;
  boost::asio::mutable_buffer b = boost::asio::buffer(output);

  while(is_running){
    if(consumer.receive(b)){
      std::cout<< output[0] << " " << output[1] << " " << output[2]  << std::endl;
    }
  }
  return 0;
}
