#include "pcam/cam_publisher.h"
#include "pcam/pcam_utils.h"

int main(int argc, char** argv){

    pcam::TCPVideoPublisher tcp_video_publiser(6060);
    pcam::CamPublisher cam_publisher(&tcp_video_publiser);
    pcam::Signal signal;

    auto stop_callback = std::bind(&pcam::CamPublisher::stop, &cam_publisher);
    signal.add_handler(stop_callback);

    std::cout<< "start" << std::endl;
    cam_publisher.run();
    std::cout<< "stop" << std::endl;

    return 0;
}
