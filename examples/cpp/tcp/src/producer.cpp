#include <thread>
#include <signal.h>

#include "pcom/tcp/pcom_tcp_producer.h"

bool is_running = true;


void sighandler(int sig)
{
    is_running = false;
}


int main(int argc, char** argv)
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT, &sighandler);

    pcom::tcp::Producer producer(6067);
    std::string msg;
    int count=0;
    while(is_running){
        std::cout<< "send: " << count << std::endl;
        msg = std::to_string(count);
        producer.send(boost::asio::buffer(msg));
        count++;
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
  return 0;
}
