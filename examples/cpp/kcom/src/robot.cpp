
#include <signal.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <thread>

#include "kcom/kcom_types.h"
#include "kcom/shared_memory.h"

using namespace std;
using namespace kcom;

bool is_running = true;

void sighandler(int sig)
{
    is_running = false;
}

void read_robot_hw_data(std::vector<double>& sensor, double count){
  for(std::size_t i=0; i < sensor.size(); i++){
    sensor[i] = 0;
  }
  sensor[sensor.size()-2]=count;
}

void sensor_writer(DataVector* data, const std::vector<double>* sensor){
  for(std::size_t i=0; i < sensor->size(); i++){
    (*data)[i+1]=(*sensor)[i];
  }
}

void command_reader(DataVector* data, std::vector<double>* command){
  for(std::size_t i=0; i < command->size(); i++){
    (*command)[i]=(*data)[i+1];
  }
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
  std::vector<double> robot_command_data(8);

  FuncDataTransfer funcRobotSensorWriter = std::bind(sensor_writer, std::placeholders::_1, &robot_sensor_data);
  FuncDataTransfer funcCommandReader = std::bind(command_reader, std::placeholders::_1, &robot_command_data);

  SharedMemory command_consumer(NameId("robot_command"), funcCommandReader);
  command_consumer.initialise(DataSize(robot_command_data.size()));

  SharedMemory sensor_producer(NameId("robot_data"), funcRobotSensorWriter);
  sensor_producer.initialise(DataSize(robot_sensor_data.size()));

  int count=0;
  std::cout<< "start robot" << std::endl;
  while(is_running){

    // read data from robot hardware
    read_robot_hw_data(robot_sensor_data, count);

    // try and write this data to shared memory buffer
    if(sensor_producer.try_write()){
        print("sent sensor: ", robot_sensor_data);
    }

    if(command_consumer.try_read(10)){
        print("received command: ", robot_command_data);
    }
    count++;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
