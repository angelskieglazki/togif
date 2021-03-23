#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <iostream>

#include "options.h"
#include "Video.h"


#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <fnmatch.h>

#include "algs/raw_to_video_alg.h"
#include "algs/video_to_gif_alg.h"
#include "algs/img_to_video_alg.h"




template<typename T, typename CreatingAlg>
void create(T& opt) {
  CreatingAlg::create(opt);
}

int main(int argc, char** argv) {
  auto options = parse_cmd_line_opt(argc, argv);
  
  std::cout << "Options: " << "\n";
  std::cout << options;


  {
    if (options.mode == kRawToVideo) {
      create<options_t, RawToVideoAlg>(options);
    } else if (options.mode == kImagesToVideo) {
      create<options_t, ImgToVideoAlg>(options);
    } else if (options.mode == kVideoToGif) {
      create<options_t, VideoToGifAlg>(options);
    }
 
  }
  
  return 0;
}
