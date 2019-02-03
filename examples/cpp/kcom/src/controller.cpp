
#include <signal.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <thread>

#include "kcom/shared_memory.h"
#include <math.h>

using namespace std;
using namespace kcom;

bool is_running = true;

void sighandler(int sig)
{
    is_running = false;
}

void robot_sensor_reader(DataVector* data, std::vector<double>* sensor){
  for(std::size_t i=0; i < sensor->size(); ++i){
     (*sensor)[i]=(*data)[i+1];
  }
}

void robot_command_writer(DataVector* data, const std::vector<double>* command){
  for(std::size_t i=0; i < command->size(); ++i){
     (*data)[i+1]=(*command)[i];
  }
}

void robot_controller(std::vector<double>&  command, double count){
  for(std::size_t i=0; i < command.size()-1; ++i){
     command[i]=std::sin(count/100.0);
  }
  command[command.size()-1]=count;
}

void print(std::string name, const std::vector<double>& v){
    std::cout<< name << " ";
    for(std::size_t i = 0; i < v.size()-1; ++i){
        std::cout<< v[i] << " ";
    }
    std::cout<<std::endl;
}


int main(int argc, char** argv)
{

  signal(SIGABRT, &sighandler);
  signal(SIGTERM, &sighandler);
  signal(SIGINT, &sighandler);

  std::vector<double> robot_sensor_data(8);
  std::vector<double> robot_command(8);

  FuncDataTransfer funcRobotSensorReader = std::bind(robot_sensor_reader, std::placeholders::_1, &robot_sensor_data);
  FuncDataTransfer funcRobotCommandWriter = std::bind(robot_command_writer, std::placeholders::_1, &robot_command);

  SharedMemory sensor_consumer(NameId("robot_data"), funcRobotSensorReader);
  SharedMemory command_producer(NameId("robot_command"), funcRobotCommandWriter);

  sensor_consumer.initialise();
  command_producer.initialise();

  int count=0;
  std::cout<< "start robot" << std::endl;
  while(is_running){

    // read data from robot (blocking)
    sensor_consumer.read(is_running);
    print("read sensor", robot_sensor_data);


    // controller
    robot_controller(robot_command, count);

    command_producer.write(is_running);
    print("send command", robot_command);
    count++;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
