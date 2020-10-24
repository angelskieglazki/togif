/**
 * \file options.h
 * \author max
 * Create on Wed 2020-10-21 21:34 UTC
 * 
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include <getopt.h>

struct options_t {
  std::string input_video_name = "";
  std::string output_gif_name = "";
  unsigned short frame_height = 320;
  unsigned short frame_width = 240;
  unsigned short skip_frame_count = 10;
  size_t gif_quality = 75;
};

std::ostream& operator<<(std::ostream& os, const options_t& opts) {
  os << "input_video_name: " << opts.input_video_name << "\n";
  os << "output_gif_name: " << opts.output_gif_name << "\n";
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
  std::cout<<"\t-g, --gif-name-out\t gif name output" << "\n";
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
  if (options.input_video_name.empty() || options.output_gif_name.empty()) {
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
      { "video-name-in", required_argument, NULL, 'v' },
      { "gif-name-out", required_argument, NULL, 'g' },
      { "frame-height", required_argument, NULL, 'h' },
      { "frame-width", required_argument, NULL, 'w' },
      { "skip-frame-count", required_argument, NULL, 's' },
      { "gif-quality", required_argument, NULL, 'q' },
      { "help", no_argument, NULL, 1},
      { 0, 0, NULL, 0}
    };

    c = getopt_long(argc, argv, "v:g:h:w:s:q:", long_options, &option_index);
    if (c == -1) break;

    switch (c) {
      case 'v':
        opts.input_video_name = optarg;
        break;

      case 'g':
        opts.output_gif_name = optarg;
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

      case 1:
      default:
        usage();
        break;
    }
  }

  if (set_the_needful(opts) != EXIT_SUCCESS) {
    usage();
    exit(EXIT_FAILURE);
  }

  return opts;
}
#endif

