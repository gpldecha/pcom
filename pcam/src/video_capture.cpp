#include "pcam/video_capture.h"

namespace pcam{

VideoCapture::VideoCapture(const StreamMedium& stream_medium):
stream_medium_(stream_medium)
{
  is_running_.store(true);
}

void VideoCapture::start(){
  thread_ = std::thread(&VideoCapture::run, this);
}

void VideoCapture::stop(){
  is_running_.store(true);
}

void VideoCapture::run(){

  // try to open video
  while(is_running_.load()){
    capture_.open(cv::CAP_ANY);
    if(capture_.isOpened())
        break;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // capture frames
  while(is_running_.load()){
    cv::Mat frame;
    capture_ >> frame;
    // send the data tcp/ipc

  }

}

}
