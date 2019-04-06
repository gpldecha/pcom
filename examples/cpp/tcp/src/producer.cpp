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
        msg = std::to_string(count) + "\n";
        //boost::asio::const_buffer bit_msg = boost::asio::buffer(msg);
        //std::cout<< "string size: " << msg.size() << std::endl;
        //std::cout<< "bit msg:     " <<  boost::asio::buffer_size(bit_msg) << std::endl;
        producer.send(boost::asio::buffer(msg));
        count++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  return 0;
}
