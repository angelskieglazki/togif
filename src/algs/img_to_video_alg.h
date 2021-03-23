/**
* \file imgtovideoalg.h
* \author max
* Created on Tue Mar 23 22:13:29 2021
* 
*/

#ifndef IMGTOVIDEOALG_H
#define IMGTOVIDEOALG_H
#include <opencv2/highgui/highgui.hpp>

#include <set>
#include <vector>
#include <iostream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;
#include "../utils.h"

class ImgToVideoAlg {
public:
  template<typename T>
  static void create(T& options) {
    std::vector<cv::Mat> images;
    images.reserve(500);
    std::set<std::string> images_names;
    for (const auto & entry : fs::directory_iterator("./ZZZ")) {
    //   if (std::regex_match(entry.path().filename().string() , video_regex) == 1) {
        //std::cout<<entry.path().filename().string()<<"\n";
        images_names.emplace(entry.path().string());
  //    }
    }

    for (auto& s : images_names) {
      std::cout<<s<<"\n";
      images.emplace_back(cv::imread(s));
    }

    int codec = cv::VideoWriter::fourcc('P','I','M','1');
    auto writer = cv::VideoWriter("out.avi", codec, 24, cv::Size(271, 195), true);


    if (!writer.isOpened()){
      std::cout  << "Could not open the output video for write: "<< std::endl;
        return;
    }

    int j = 0;
    for (auto& img : images){
      for (int i=0; i<23; ++i) {
        writer.write(img);
      }
    }

    
    writer.release();
  }
};

#endif
