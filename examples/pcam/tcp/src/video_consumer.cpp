#include "pcam/cam_subscriber.h"
#include "pcam/pcam_utils.h"

bool is_runing = true;

void interrupt_handler(){
    is_runing = false;
}

int main(int argc, char** argv){

    pcam::TCPVideoSubscriber tcp_video_subscriber("127.0.0.1", 6060);
    pcam::CamSubscriber cam_subscriber(&tcp_video_subscriber);
    pcam::Signal signal;

    auto stop_callback = std::bind(interrupt_handler);
    signal.add_handler(stop_callback);

    cv::Mat frame;
    while(is_runing){
        cam_subscriber.try_read(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
