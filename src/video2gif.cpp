#include <string>
#include <iostream>

#include "options.h"
#include "Video.h"

int main(int argc, char** argv) {
  auto options = parse_cmd_line_opt(argc, argv);
  
  std::cout << "Options: " << "\n";
  std::cout << options;

  Video video{
    options.input_video_name,
    options.output_gif_name,
    options.frame_height,
    options.frame_width,
    options.skip_frame_count,
    options.gif_quality
  };

  video.create_gif();
  return 0;
}
