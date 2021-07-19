/**
 * \file Video.cpp
 * \author max
 * Create on Wed 2020-10-21 21:39 UTC
 * 
 */

#include "Video.h"

void Video::extract_frames() {
  try{
    cv::VideoCapture cap(this->video_name);
    if (!cap.isOpened()) {
      CV_Error( cv::Error::StsError , "Can't open video file");
    }

    double fIdx = 0;
    double frnb(cap.get(cv::CAP_PROP_FRAME_COUNT));

    std::cout << "\nFrames count = " << frnb<< "\n\n";
    std::cout << "Start frames extracting:" << "\n";

    float progress = 0.0;

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

      int barWidth = 70;

      std::cout << "[";
      int pos = (barWidth * progress)/100;
      for (int i = 0; i < barWidth; ++i) {
          if (i < pos) std::cout << "=";
          else if (i == pos) std::cout << ">";
          else std::cout << " ";
      }
      std::cout << "] " << int(progress) << " %\r";
      std::cout.flush();
    }
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


void Video::create_gif() {
  extract_frames();

  std::cout << "\ngif creating..." << "\n";
  for(auto &frame : this->frames) {
    this->magick_frames.push_back(Video::mat_to_magick(frame));
  }
  Magick::writeImages(this->magick_frames.begin(), this->magick_frames.end(), this->output_gif_name);
}



