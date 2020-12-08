#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <iostream>

#include "options.h"
#include "Video.h"

#include "GifCreator.h"

#include <regex>
#include <filesystem>
namespace fs = std::filesystem;

#include <opencv2/highgui/highgui.hpp>
#include <fstream>
int main(int argc, char** argv) {
  auto options = parse_cmd_line_opt(argc, argv);
  
//  std::cout << "Options: " << "\n";
//  std::cout << options;

  const std::regex video_regex(options.input_video_name);

//  std::string path = "./ZZZ";


// video creating from buffer
  {
  std::vector<cv::Mat> images;
  images.reserve(500);
  std::set<std::string> images_names;
  for (const auto & entry : fs::directory_iterator("./RAW")) {
    if (std::regex_match(entry.path().filename().string() , video_regex) == 1) {
      images_names.emplace(entry.path().string());
    }
  }

  int i = 0;
  int codec = cv::VideoWriter::fourcc('P','I','M','1');
  auto writer = cv::VideoWriter("raw.avi", codec, 24, cv::Size(302, 115), true);

  if (!writer.isOpened()){
    std::cout  << "Could not open the output video for write: "<< std::endl;
      return -1;
  }

  for (auto& s : images_names) {
    std::cout<<s<<"\n";
    std::ifstream input(s, std::ios::binary);

    std::vector<uint8_t> bytes(
         (std::istreambuf_iterator<char>(input)),
         (std::istreambuf_iterator<char>()));

    std::cout<<bytes.size()<<"\n";
    std::vector<uint8_t> rgb_bytes;
    for (size_t i = 1; i <= bytes.size(); ++i) {
      if ( i % 4 == 0) continue;
      rgb_bytes.push_back(bytes[i-1]);
    }
    std::cout<<rgb_bytes.size()<<"\n";
    cv::Mat img(115, 302, CV_8UC3, rgb_bytes.data());
    for (int i=0; i<23; ++i) {
      writer.write(img);
    }
    cv::imwrite(std::to_string(i)+"s.png", img);
    input.close();
    ++i;
  }


  
  writer.release();
  
  }
  
  return 0;


//video creating

  std::vector<cv::Mat> images;
  images.reserve(500);
  std::set<std::string> images_names;
  for (const auto & entry : fs::directory_iterator("./ZZZ")) {
    if (std::regex_match(entry.path().filename().string() , video_regex) == 1) {
      //std::cout<<entry.path().filename().string()<<"\n";
      images_names.emplace(entry.path().string());
    }
  }

  for (auto& s : images_names) {
    std::cout<<s<<"\n";
    images.emplace_back(cv::imread(s));
  }

  int codec = cv::VideoWriter::fourcc('P','I','M','1');
  auto writer = cv::VideoWriter("out.avi", codec, 24, cv::Size(271, 195), true);


  if (!writer.isOpened()){
    std::cout  << "Could not open the output video for write: "<< std::endl;
      return -1;
  }

  int j = 0;
  for (auto& img : images){
    for (int i=0; i<23; ++i) {
      writer.write(img);
    }
  }

  
  writer.release();
  return 0;

  GifCreator gif_creator;
  size_t idx = 0;
  for (const auto & entry : fs::directory_iterator("."))
    if (std::regex_match(entry.path().filename().string() , video_regex) == 1) {
          
          gif_creator.add_video(std::move(std::make_unique<Video>(
            idx,
            entry.path().filename().string(),
            options.frame_height,
            options.frame_width,
            options.skip_frame_count,
            options.gif_quality
            )));
        ++idx;
    }

  gif_creator.start_gif_creating();
  move_down(idx);
  std::cout<<"\n";
  return 0;
}
