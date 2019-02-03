#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <signal.h>

#include "pcom/ipc/pcom_producer.h"
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

  //std::array<int, 1> input = {1};
  std::vector<double> input(1); input[0] = 1;
  Producer producer(NameId("my_data"), NumItems(5), ItemSize(get_size(input)));

  int count=0;
  std::cout<< "Start producing" << std::endl;
  while(is_running){
    input[0] = count;
    std::cout<< "send: " << input[0] << std::endl;
    if(producer.send(boost::asio::buffer(input))){
      count++;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  std::cout<< "exit" << std::endl;

  return 0;
}
