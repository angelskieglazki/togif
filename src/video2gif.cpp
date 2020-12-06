#include <memory>
#include <string>
#include <iostream>

#include "options.h"
#include "Video.h"

#include "GifCreator.h"

#include <regex>
#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char** argv) {
  auto options = parse_cmd_line_opt(argc, argv);
  
  std::cout << "Options: " << "\n";
  std::cout << options;

  const std::regex video_regex(options.input_video_name);

  std::string path = ".";

  GifCreator gif_creator;
  size_t idx = 0;
  for (const auto & entry : fs::directory_iterator("."))
    if (std::regex_match(entry.path().filename().string() , video_regex) == 1) {
          
          std::cout<<entry.path().filename().string()<<"\n";
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
  std::cout<<"\n";
  return 0;
}
