#ifndef PCAM_CAM_PUBLISHER_H_
#define PCAM_CAM_PUBLISHER_H_

//stl
#include <thread>
#include <chrono>
#include <atomic>

// opencv
#include <opencv2/opencv.hpp>

// library
#include "pcam/pcam_types.h"

namespace pcam{

class CamPublisher{

public:

  CamPublisher(VideoPublisher *video_streamer);

  void start();

  void stop();

  void run();

private:

  VideoPublisher* video_streamer_;
  std::vector<unsigned char> bytes_;
  cv::VideoCapture capture_;
  std::thread thread_;

public:

  std::atomic_bool is_running_;

};


}

#endif
