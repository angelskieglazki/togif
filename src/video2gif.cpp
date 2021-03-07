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
#include <fnmatch.h>

namespace gvmaker_utils {
std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
}

class RawToVideoAlg {
public:
  template<typename T>
  static void create(T& options) {
    std::vector<cv::Mat> images;
    images.reserve(500);
    std::set<std::string> images_names;


    for (const auto & entry : fs::directory_iterator(options.workpath)) {
      if (fnmatch(options.wildcard_regex.c_str(), entry.path().filename().string().c_str(), 0) == 0) {
        std::cout<<entry.path().parent_path()<<" "<<entry.path().c_str()<<"   "<<entry.path().filename().string()<<'\n';
        images_names.emplace(entry.path().filename().string());
      }
    }

    auto firstname = std::begin(images_names);
    auto arr = gvmaker_utils::split(*firstname, '.');
    const std::regex size_regex("[0-9]+x[0-9]+");
    std::string out_video_name;
    size_t w = 0;
    size_t h = 0;
    
    for (auto& p : arr) {
      if (std::regex_match(p , size_regex) == 1) {
        auto wh = gvmaker_utils::split(p, 'x');
        w = std::atoi(wh[0].c_str());
        h = std::atoi(wh[1].c_str());
      } else if (p.find("Path") == std::string::npos){
        out_video_name.append(p).append(1,'.');
      }
    }
    out_video_name.append("avi");
    std::cout<<"W: "<<w<<'\n';
    std::cout<<"H: "<<h<<'\n';
    std::cout<<"OUTNAME: "<<out_video_name<<'\n';
  int i = 0;
  int codec = cv::VideoWriter::fourcc('P','I','M','1');
  auto writer = cv::VideoWriter(out_video_name, codec, 24, cv::Size(w, h), true);

  if (!writer.isOpened()){
    std::cout  << "Could not open the output video for write: "<< std::endl;
    //need exception
//      return -1;
  }

  for (auto& s : images_names) {
    std::cout<<s<<"\n";
    std::ifstream input(options.workpath+"/"+s, std::ios::binary);

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
    cv::Mat img(h, w, CV_8UC3, rgb_bytes.data());
//     cv::imwrite(std::to_string(i)+"s.png", img);
    for (int i=0; i<23; ++i) {
      writer.write(img);
    }
//    images.emplace_back(cv::imread(std::to_string(i)+"s.png"));
    input.close();
    ++i;
  }

  writer.release();
  }
};

template<typename T, typename CreatingAlg>
void create(T& opt) {
  CreatingAlg::create(opt);
}

int main(int argc, char** argv) {
  auto options = parse_cmd_line_opt(argc, argv);
  
  std::cout << "Options: " << "\n";
  std::cout << options;

//  const std::regex video_regex(options.input_video_name);

//  std::string path = "./ZZZ";


// video creating from buffer
  {
    if (options.mode == kRawToVideo) {
      create<options_t, RawToVideoAlg>(options);
    }
 
  }
  
  return 0;


//video creating

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
//    if (std::regex_match(entry.path().filename().string() , video_regex) == 1) {
          
          gif_creator.add_video(std::move(std::make_unique<Video>(
            idx,
            entry.path().filename().string(),
            options.frame_height,
            options.frame_width,
            options.skip_frame_count,
            options.gif_quality
            )));
        ++idx;
//    }

  gif_creator.start_gif_creating();
  move_down(idx);
  std::cout<<"\n";
  return 0;
}
