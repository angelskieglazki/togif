/**
 * \file options.h
 * \author max
 * Create on Wed 2020-10-21 21:34 UTC
 * 
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <getopt.h>
struct options_t {
  std::string input_video_name;
  std::string output_gif_name;
  unsigned short frame_height = 320;
  unsigned short frame_width = 240;
  unsigned short skip_frame_count = 10;
  size_t gif_quality = 75;
};

options_t parse_cmd_line_opt(int argc, char** argv) {
  options_t opts;

  return opts;
}
#endif

