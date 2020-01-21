#ifndef PCAM_UTILS_H_
#define PCAM_UTILS_H_

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include <memory>

namespace pcam{

typedef std::shared_ptr<std::function<void()>> sharedFunction;

class Signal{

    public:

    Signal(){}

    void init(){
        sigIntHandler.sa_handler = &Signal::handle;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, NULL);
    }

     void add_handler(std::function<void()> func){
        callbacks.push_back(func);
    }

    private:

     static void handle(int signum){
         instance.handle_callback(signum);
     }

    void handle_callback(int signum){
        for(std::size_t i = 0; i < callbacks.size();i++){
            (callbacks[i])();
         }
    }

    private:

        struct sigaction sigIntHandler;
        std::vector<std::function<void()>> callbacks;
        static Signal instance;

};
}


#endif
