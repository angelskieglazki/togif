/**
 * \file options.h
 * \author max
 * Create on Wed 2020-10-21 21:34 UTC
 * 
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <cstring>
#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include <getopt.h>

enum opt_error_t {
  kModeNotSet   = 1,
  kModeWrongSet = 2,
};

enum opt_mode_t {
  kNoMode         = 0x00,
  kVideoToGif     = 0x01,
  kRawToVideo     = 0x02,
  kImagesToVideo  = 0x04
};

struct options_t {
  int mode = kNoMode;
  std::string input_video_name;
  std::string workpath = "./";
  std::string wildcard_regex;
  unsigned short frame_height = 240;
  unsigned short frame_width = 320;
  unsigned short skip_frame_count = 10;
  size_t gif_quality = 75;
  std::string input_regex;
};

std::ostream& operator<<(std::ostream& os, const options_t& opts) {
  os << "Mode: " << opts.mode << '\n';
  os << "input_video_name: " << opts.input_video_name << "\n";
  os << "frame_height "  << opts.frame_height << "\n";
  os << "frame_width "  << opts.frame_width << "\n";
  os << "skip_frame_count "  << opts.skip_frame_count << "\n";
  os << "gif_quality "  << opts.gif_quality << "\n";
  return os;
}

void usage() {
  std::cout<<"usage: " << "\n";
  std::cout<<"\ttogif [v:g:h:w:s:q:] " << "\n";
  std::cout<<"required: " << "\n";
  std::cout<<"\t-v, --video-name-in\t video name input" << "\n";
  std::cout<<"optional: " << "\n";
  std::cout<<"\t-h, --frame-height\t" << "\n";
  std::cout<<"\t-w, --frame-width\t" << "\n";
  std::cout<<"\t-s, --skip-frame-count\t" << "\n";
  std::cout<<"\t-q, --gif-quality\t" << "\n";
}


/**
 * @brief Check that video inputname and gif output name sets correctly
 *
 * @param options
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int set_the_needful(const options_t& options) {
  if (options.mode == kNoMode) {
    return kModeNotSet;    
  }

  if (options.mode != kVideoToGif ||
      options.mode != kRawToVideo ||
      options.mode != kImagesToVideo) {
    return kModeWrongSet;
  }

  if (options.input_video_name.empty()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


/**
 * @brief Parse command line params
 *
 * @param argc
 * @param argv
 *
 * @return options_t structure that contain program options
 */
options_t parse_cmd_line_opt(int argc, char** argv) {
  options_t opts;
  
  int c;

  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      { "mode",             required_argument, NULL, 'm'},
      { "video-name-in",    required_argument, NULL, 'v' },
      { "work-path",        required_argument, NULL, 'd' },
      { "frame-height",     required_argument, NULL, 'h' },
      { "frame-width",      required_argument, NULL, 'w' },
      { "skip-frame-count", required_argument, NULL, 's' },
      { "gif-quality",      required_argument, NULL, 'q' },
      { "help",             no_argument,       NULL,  1  },
      { "wildcard-reg",     required_argument, NULL,  0  },
      { 0,                  0,                 NULL,  0  }
    };

    c = getopt_long(argc, argv, "m:v:d:h:w:s:q:", long_options, &option_index);
    if (c == -1) break;

    switch (c) {
      case 'm':
        try {
          opts.mode = std::atoi(optarg);
        } catch(const std::exception& e) {
          std::cerr << e.what() << '\n';
        }
        break;

      case 'v':
        opts.input_video_name = optarg;
        std::cout<<"OPTION: "<<opts.input_video_name<<'\n';
        std::cout<<"OPTION: "<<optarg<<'\n';
        break;
      case 'd':
        opts.workpath = optarg;
        std::cout<<opts.workpath<<'\n';
        break;

      case 'h':
        try {
          opts.frame_height = std::atoi(optarg);
        } catch(const std::exception& e) {
          std::cerr << e.what() <<"\n";
        }
        break;

      case 'w':
        try {
          opts.frame_width = std::atoi(optarg);
        } catch(const std::exception& e) {
          std::cerr << e.what() << "\n";
        }
        break;

      case 's':
        try {
          opts.skip_frame_count = std::atoi(optarg);
        } catch(const std::exception& e) {
          std::cerr << e.what() << "\n";
        }
        break;

      case 'q':
        try {
          opts.gif_quality = std::atoi(optarg);
        } catch(const std::exception& e) {
          std::cerr << e.what() << "\n";
        }
        break;

      case 0:
        if (long_options[option_index].flag != 0) break;
        if (strcmp(long_options[option_index].name, "wildcard-reg") == 0) {
          opts.wildcard_regex = optarg;
          std::cout<<"Wildcard: "<<opts.wildcard_regex<<'\n';
        }
        break;

      case 1:
      default:
        usage();
        break;
    }
  }

//  if (set_the_needful(opts) != EXIT_SUCCESS) {
//    usage();
//    exit(EXIT_FAILURE);
//  }

  return opts;
}
#endif

