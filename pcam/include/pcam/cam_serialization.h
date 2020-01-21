#ifndef PCAM_CAM_SERIALIZATION_H_
#define PCAM_CAM_SERIALIZATION_H_

// opencv
#include <opencv2/opencv.hpp>
#include <bitset>

namespace pcam {



template <typename T>
inline void to_binary(T value, std::vector<unsigned char>::iterator it, bool debug=false){
    if(sizeof(T) == 1){
        *it=value;
    }else{
        std::bitset<sizeof(T)*8> bit(value);
        std::bitset<sizeof(T)*8> mask;
        mask.set();
        mask >>= 8*(sizeof(T)-1);
        for(unsigned int i = 0; i < sizeof(T); i++){
            *it=(bit & mask).to_ulong();
            if(debug){
                std::cout<< static_cast<int>(*it) << std::endl;
            }
            it++;
            bit >>= 8;
        }
    }
}

template<typename T>
inline T from_binary(std::vector<unsigned char>::iterator it, bool debug=false)
{
    if(sizeof(T) == 1){
        return static_cast<T>(*it);
    }else{
        std::bitset<sizeof(T)*8> final_value;
        for(unsigned int i = 0; i < sizeof(T)-1; i++){
            if(debug){
                std::cout<< static_cast<int>(*it) << std::endl;
            }
           final_value |= (std::bitset<sizeof(T)*8>(*it) << i*8);
           it++;
        }
        final_value |= (std::bitset<sizeof(T)*8>(*it) << (sizeof(T)-1)*8);
        return static_cast<T>(final_value.to_ulong());
    }
}

inline void serialise_cv(const cv::Mat& frame, std::vector<unsigned char>& data){
    data.resize(frame.total() + 4*sizeof(int) + 1);
    std::vector<unsigned char>::iterator it = data.begin();
    to_binary<int>(frame.rows, it, true);
    to_binary<int>(frame.cols, it);
    to_binary<int>(frame.channels(), it);
    to_binary<int>(frame.type(), it);
    uchar* itd = frame.data;
    for(std::size_t i = 0; i < frame.total(); i++){
        *it = *itd;
        itd++;
        it++;
    }
    data[data.size()-1] = '\n';
}

inline void deserialise_cv(std::vector<unsigned char>& data, cv::Mat& frame){
    if(data.size() == 307216){
        std::vector<unsigned char>::iterator it = data.begin();
        int rows = from_binary<int>(it, true);
        int cols = from_binary<int>(it);
        int channels = from_binary<int>(it);
        int type = from_binary<int>(it);
        std::cout<< rows << " x " << cols << " x " << channels << std::endl;
    }

}

}

#endif
