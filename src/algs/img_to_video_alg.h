/**
* \file imgtovideoalg.h
* \author max
* Created on Tue Mar 23 22:13:29 2021
*/

#ifndef IMGTOVIDEOALG_H
#define IMGTOVIDEOALG_H

#include <bits/c++config.h>
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
    std::set<std::string> images_names;

    std::size_t s = 0;

    for (const auto& entry : fs::directory_iterator(options.workpath)) {
      if (fnmatch(options.wildcard_regex.c_str(), entry.path().filename().string().c_str(), 0) == 0) {
        std::cout<<entry.path().filename().string()<<"\n";
        images_names.emplace(entry.path().filename().string());
        ++s;
      }
    }

    images.reserve(s);

    for (auto& s : images_names) {
      std::cout<<s<<"\n";
      images.emplace_back(cv::imread(options.workpath+"/"+s));
    }

    auto firstname = std::begin(images_names);
    auto arr = split(*firstname, '.');
    const std::regex num_regex("[0-9]+");
    std::string out_video_name;
    
    for (const auto& p : arr) {
      if ((p.find("Path") == std::string::npos) && (std::regex_match(p, num_regex) == 0)) {
        out_video_name.append(p).append(1,'.');
      }
    }
    auto firstimage = images[0];
    std::size_t w = firstimage.cols;
    std::size_t h = firstimage.rows;
    out_video_name.append("avi");
    std::cout<<"W: "<<w<<'\n';
    std::cout<<"H: "<<h<<'\n';
    std::cout<<"OUTNAME: "<<out_video_name<<'\n';


    int codec = cv::VideoWriter::fourcc('P','I','M','1');
    auto writer = cv::VideoWriter(out_video_name, codec, 24, cv::Size(w, h), true);


    if (!writer.isOpened()){
      std::cout  << "Could not open the output video for write: "<< std::endl;
      //need exeption
//      return;
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
