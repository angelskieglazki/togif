/**
 * \file Video.cpp
 * \author max
 * Create on Wed 2020-10-21 21:39 UTC
 * 
 */
#include <thread> 
#include "Video.h"

std::ostream& operator<<(std::ostream& os, const Video& v) {
  os << "input_video_name: " << v.video_name << "\n";
  os << "frame_height "  << v.frame_height << "\n";
  os << "frame_width "  << v.frame_width << "\n";
  os << "skip_frame_count "  << v.skip_frame_count << "\n";
  os << "gif_quality "  << v.gif_quality << "\n";
  return os;
}

void Video::extract_frames() {
  try{
    cv::VideoCapture cap(this->video_name);
    if (!cap.isOpened()) {
      CV_Error( cv::Error::StsError , "Can't open video file");
    }

    double fIdx = 0;
    double frnb(cap.get(cv::CAP_PROP_FRAME_COUNT));

//    std::cout << "\nFrames count = " << frnb<< "\n\n";
//    std::cout << "Start frames extracting:" << "\n";

    float progress = 0.0;
    multiprogress_bar->enable(idx);
    multiprogress_bar->set_prefix(idx, "extracting frames");
    for (;;) {
//      std::cout<<"frame : "<<fIdx<<std::endl;
      cv::Mat frame;
       
      if (fIdx < 0 || fIdx >= frnb) break;

      cap.set(cv::CAP_PROP_POS_FRAMES, fIdx);
      bool success = cap.read(frame);

      if (success) {
        this->frames.push_back(frame);
        fIdx += skip_frame_count;
        progress = (fIdx/frnb)*100;
      } else {
        break;
      }

      multiprogress_bar->update(progress, idx);
    }
  
    multiprogress_bar->disable(idx);
  } catch(cv::Exception& e){
    std::cerr << e.msg << std::endl;
    exit(1);
  }
}


Magick::Image Video::mat_to_magick(cv::Mat& src) {
  Magick::Image image(src.cols, src.rows, "BGR", Magick::CharPixel, (char *)src.data);
  image.resize({frame_width, frame_height});
  image.compressType(Magick::JPEGCompression);
  image.quality(gif_quality);
  return image;
}


int Video::create_gif() {
  extract_frames();
;
//  std::cout << "\ngif creating..." << "\n";
  for(auto &frame : this->frames) {
    this->magick_frames.push_back(Video::mat_to_magick(frame));
  }
  Magick::writeImages(this->magick_frames.begin(), this->magick_frames.end(), this->output_gif_name);


  multiprogress_bar->enable(idx);
  multiprogress_bar->set_prefix(idx, "gif creating");

  for (size_t i = 1; i <= 100; ++i) {
    multiprogress_bar->update(i, idx);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  multiprogress_bar->disable(idx);
  return 1;
}



