#include "pcam/cam_subscriber.h"
#include "pcam/cam_serialization.h"


namespace pcam{

CamSubscriber::CamSubscriber(VideoSubscriber *video_subscriber):
video_subscriber_(video_subscriber){}

bool CamSubscriber::try_read(cv::Mat& frame){
    if( video_subscriber_->receive(data) ){
        // de-serialize the data
        // rows, cols, depth
        // 2 bytes
        deserialise_cv(data, frame);

        return true;
    }else{
        return false;
    }
}

}
