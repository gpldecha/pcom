#ifndef PCAM_VDIEO_CAPTURE_H_
#define PCAM_VDIEO_CAPTURE_H_

//stl
#include <thread>
#include <chrono>
#include <atomic>

// opencv
#include <opencv2/opencv.hpp>

// library
#include "pcam/pcam_types.h"

namespace pcam{

class VideoCapture{

public:

  VideoCapture(const StreamMedium& stream_medium);

  void start();

  void stop();

private:

  void run();

private:

  StreamMedium stream_medium_;
  cv::VideoCapture capture_;
  std::thread thread_;

public:

  std::atomic_bool is_running_;

};


}

#endif
