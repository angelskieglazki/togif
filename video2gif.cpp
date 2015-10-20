#include <stdio.h>
#include <string>
#include <iostream>
#include <Magick++.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
using namespace Magick;
class Video{
public:
  Video(const std::string& video_n){ video_name = video_n; };
  ~Video() {};
  void extract_frames();
  void save_frames();
  void create_gif();
//  std::vector<Magick::Image> Magick_frames;
private:
  std::string video_name;
  std::vector<Mat> frames;
  std::vector<Magick::Image> Magick_frames;
  static inline Magick::Image mat_2_magick(cv::Mat& src);
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

inline Magick::Image Video::mat_2_magick(cv::Mat& src) {
  Magick::Image mgk(src.cols, src.rows, "BGR", Magick::CharPixel, (char *)src.data);
  return mgk;
}

void Video::create_gif() {
  for(std::vector<cv::Mat>::iterator frame = this->frames.begin(); frame != this->frames.end(); ++frame){
    this->Magick_frames.push_back(Video::mat_2_magick(*frame));
  }
  Magick::writeImages(this->Magick_frames.begin(), this->Magick_frames.end(), "hui.gif");
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
  //myVideo.save_frames();
  myVideo.create_gif();

   /* std::vector<Magick::Image> _frames;
    Magick::Image img1( "100x100", "white" );
    img1.pixelColor( 49, 49, "red" );
    _frames.push_back(img1);
    
    Magick::Image img2( "100x100", "red" );
    img2.pixelColor( 49, 49, "white" );
    _frames.push_back(img2);
    
    img1.animationDelay(2000);
    img2.animationDelay(2000);
    
    Magick::writeImages(_frames.begin(), _frames.end(), "2.gif");
    */
      return 0;
}
