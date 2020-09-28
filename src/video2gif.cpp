#include <string>
#include <iostream>
#include <Magick++.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
using namespace Magick;
class Video{
public:
  Video(const std::string& video_n, const std::string& gif_n) :
      video_name(video_n),
      output_gif_name(gif_n)
  {}
  ~Video() {}
 void create_gif();
private:
  std::string video_name;
  std::string output_gif_name;
  std::vector<Mat> frames;
  std::vector<Magick::Image> Magick_frames;
  void extract_frames();
  static inline Magick::Image mat_2_magick(cv::Mat& src);
};

void Video::extract_frames(){
  try{
    VideoCapture cap(this->video_name);
    if (!cap.isOpened()) CV_Error(CV_StsError, "Can't open video file");
    double fIdx = 0;
    double frnb(cap.get(CV_CAP_PROP_FRAME_COUNT));
   // std::cout << "frame count = " << frnb<< std::endl;
    for (;;){
     // std::cout<<"frame : "<<fIdx<<std::endl;
      Mat frame;
      if (fIdx < 0 || fIdx >= frnb) break;
      cap.set(CV_CAP_PROP_POS_FRAMES, fIdx);
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
  mgk.quality(50);
  return mgk;
}

void Video::create_gif() {
  extract_frames();
  for(auto &frame : this->frames) {
    this->Magick_frames.push_back(Video::mat_2_magick(frame));
  }
  Magick::writeImages(this->Magick_frames.begin(), this->Magick_frames.end(), this->output_gif_name);
}

int main(int argc, char** argv){
  if (argc >= 3){
    Video myVideo(argv[1], argv[2]);
    myVideo.create_gif();
  } else std::cout << "Error argument!\n";
  return 0;
}
