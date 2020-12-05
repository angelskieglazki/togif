#include <string>
#include <iostream>

#include "options.h"
#include "Video.h"
#include "thread_pool.h"

#include <regex>
#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char** argv) {
  auto options = parse_cmd_line_opt(argc, argv);
  
  std::cout << "Options: " << "\n";
  std::cout << options;

  const std::regex video_regex(options.input_video_name);

  std::string path = ".";

  std::vector<Video> videos;
  for (const auto & entry : fs::directory_iterator("."))
    if (std::regex_match(entry.path().filename().string() , video_regex) == 1) {
          Video video{ entry.path().filename().string(),
            options.frame_height,
            options.frame_width,
            options.skip_frame_count,
            options.gif_quality
            };
          videos.emplace_back(video);
    }

  std::cout<<"\n";
  
  thread_pool pool(2);
  std::vector< std::future<int> > results;

  int c = 0;
  for (auto& v : videos) {    
    results.emplace_back(
          pool.add_to_thread_pool([&] {
              return v.create_gif(c);
            })
        );
    ++c;
  }




  return 0;
}
