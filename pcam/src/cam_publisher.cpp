#include "pcam/cam_publisher.h"
#include "pcam/cam_serialization.h"

namespace pcam{

CamPublisher::CamPublisher(VideoPublisher* video_steamer):
video_streamer_(video_steamer)
{
  is_running_.store(true);
}

void CamPublisher::start(){
  thread_ = std::thread(&CamPublisher::run, this);
}

void CamPublisher::stop(){
  is_running_.store(true);
}

void CamPublisher::run(){

  // try to open video
  while(is_running_.load()){
    capture_.open(cv::CAP_ANY);
    if(capture_.isOpened()){
        std::cout<< "Camera open" << std::endl;
        break;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // capture frames
  while(is_running_.load()){
    cv::Mat frame;
    if(capture_.read(frame)){
        //cv::imshow( "Frame", frame );
        //cv::waitKey(1);

        // send the data tcp/ipc
        pcam::serialise_cv(frame, bytes_);
        std::cout<< "size of bites to send: " << bytes_.size() << std::endl;
        video_streamer_->send(boost::asio::buffer(bytes_));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }else{
        std::cout<< "unable to read frame" << std::endl;
    }


  }

}

}
