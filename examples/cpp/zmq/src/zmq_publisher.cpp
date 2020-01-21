#include <zmq.hpp>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>
#include <boost/format.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool is_running = true;

void sighandler(int sig)
{
    is_running = false;
}

int main(int argc, char** argv){

    signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
    signal(SIGINT, &sighandler);

    json j;


    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://0.0.0.0:5558");
    //publisher.bind("ipc://example");

    std::cout<< "Start" << std::endl;
    int n = 0;


    while(is_running){

        zmq::message_t env1(1);
        memcpy(env1.data(), "B", 1);

        j["position"] = {1, 1, 1};
        j["velocity"] = {0, 0, 0};
        j["time"] = n;
        std::string msg1_str = j.dump();
        zmq::message_t msg1(msg1_str.size());

        memcpy(msg1.data(), msg1_str.c_str(), msg1_str.size());
        publisher.send(env1, ZMQ_SNDMORE);
        publisher.send(msg1);

        n += 1;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
    std::cout<< "Start" << std::endl;
  return 0;
}
