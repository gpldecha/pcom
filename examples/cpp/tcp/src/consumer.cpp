#include <thread>
#include "pcom/tcp/pcom_tcp_consumer.h"

bool is_running = true;


void sighandler(int sig)
{
    is_running = false;
}


int main(int argc, char** argv)
{

    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT,  &sighandler);

    pcom::tcp::Consumer consumer("127.0.0.1", 6067);
    std::string msg;
    while(is_running)
    {
        if(consumer.receive(msg)){
            std::cout<< msg << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
