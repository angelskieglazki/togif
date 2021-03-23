/**
* \file videotogifalg.h
* \author max
* Created on Tue Mar 23 22:16:25 2021

* Empty License
* 
*/

#ifndef VIDEOTOGIFALG_H
#define VIDEOTOGIFALG_H
#include <filesystem>

#include "../GifCreator.h"
namespace fs = std::filesystem;

class VideoToGifAlg {
public:
  template<typename T>
  static void create(T& options) {
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
  }
};

#endif
