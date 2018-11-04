#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include "pcom/pcom_producer.h"
#include "pcom/pcom_utils.h"
#include <signal.h>

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

  std::array<double, 3> input = {1};
  Producer producer(NameId("array_data"), NumItems(10), ItemSize(get_size(input)));

  std::cout<< "Start producing" << std::endl;
  int count=0;
  while(is_running){
    input[0] = count; input[1] = count; input[2] = count;
    producer.send(boost::asio::buffer(input));
    count++;
    std::this_thread::sleep_for(std::chrono::microseconds(1));
  }

  return 0;
}
