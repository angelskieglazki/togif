/**
 * \file Video.cpp
 * \author max
 * Create on Wed 2020-10-21 21:39 UTC
 * 
 */

#include "Video.h"

using namespace cv;
using namespace std;
using namespace Magick;

void Video::extract_frames(){
  try{
    VideoCapture cap(this->video_name);
    if (!cap.isOpened()) CV_Error( cv::Error::StsError , "Can't open video file");
    double fIdx = 0;
    double frnb(cap.get(cv::CAP_PROP_FRAME_COUNT));
    std::cout << "frame count = " << frnb<< std::endl;
    for (;;){
      std::cout<<"frame : "<<fIdx<<std::endl;
      Mat frame;
      if (fIdx < 0 || fIdx >= frnb) break;
      cap.set(cv::CAP_PROP_POS_FRAMES, fIdx);
      bool success = cap.read(frame);
      if (success) { this->frames.push_back(frame); fIdx = fIdx + 10;}
      else break;
    }
  }
  catch(cv::Exception& e){
    cerr << e.msg << std::endl;
    exit(1);
  }
}

inline Magick::Image Video::mat_2_magick(cv::Mat& src) {
  Magick::Image mgk(src.cols, src.rows, "BGR", Magick::CharPixel, (char *)src.data);
  mgk.compressType(JPEGCompression);
  mgk.quality(5);
  return mgk;
}

void Video::create_gif() {
  extract_frames();
  for(auto &frame : this->frames) {
    this->Magick_frames.push_back(Video::mat_2_magick(frame));
  }
  Magick::writeImages(this->Magick_frames.begin(), this->Magick_frames.end(), this->output_gif_name);
}



