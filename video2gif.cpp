#include <stdio.h>
#include <string>
#include <iostream>
#include <Magick++.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Video{
public:
  Video(const std::string& video_n){ video_name = video_n; };
  ~Video() {};
  void extract_frames();
  void save_frames();
private:
  std::string video_name;
  std::vector<Mat> frames;
};

void Video::extract_frames(){
  try{
    VideoCapture cap(this->video_name);
    if (!cap.isOpened()) CV_Error(CV_StsError, "Can't open video file");

    for(int frame_num = 0; frame_num < cap.get(CV_CAP_PROP_FRAME_COUNT); frame_num++){
      Mat frame;
      cap >> frame;
      this->frames.push_back(frame);
    }
  }
  catch(cv::Exception& e){
    cerr << e.msg << std::endl;
    exit(1);
  }
}

void Video::save_frames() {
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
  compression_params.push_back(100);
  int frame_number = 0;
  for(std::vector<Mat>::iterator frame = this->frames.begin(); frame != this->frames.end(); ++frame){
    string file_path = "bilyat/" + std::to_string(frame_number) + ".jpg";
    imwrite(file_path, *frame, compression_params);
    frame_number++;
  }
}
int main(int argc, char** argv){
  Video myVideo("hui.mp4");
  myVideo.extract_frames();
  myVideo.save_frames();
  return 0;
}
