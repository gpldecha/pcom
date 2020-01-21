#ifndef PCAM_CAM_SUBSCRIBER_H_
#define PCAM_CAM_SUBSCRIBER_H_

//stl
#include <thread>
#include <chrono>
#include <atomic>

// opencv
#include <opencv2/opencv.hpp>

// library
#include "pcam/pcam_types.h"

namespace pcam{

class CamSubscriber{

public:

  CamSubscriber(VideoSubscriber *video_subscriber);

  bool try_read(cv::Mat& frame);

private:

  VideoSubscriber* video_subscriber_;
  std::vector<unsigned char> data;

};


}

#endif
