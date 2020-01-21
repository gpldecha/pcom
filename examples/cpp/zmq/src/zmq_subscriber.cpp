#include <zmq.hpp>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>

bool is_running = true;

void sighandler(int sig)
{
    is_running = false;
}

int main(int argc, char** argv){

    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT, &sighandler);

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "B", 1);

    try{
        subscriber.connect("tcp://0.0.0.0:5556");
    }catch(zmq::error_t& e){
        std::cout<< e.what() << std::endl;
        return -1;
    }

    std::cout<< "Start" << std::endl;
    while(is_running){
        zmq::message_t env;
        subscriber.recv(&env);
        std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
        std::cout << "Received envelope '" << env_str << "'" << std::endl;

        zmq::message_t msg;
        subscriber.recv(&msg);
        std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
        std::cout << "Received '" << msg_str << "'" << std::endl;

    }
    std::cout<< "Exit" << std::endl;
  return 0;
}
