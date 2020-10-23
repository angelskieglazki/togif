/**
 * \file Video.h
 * \author max
 * Create on Wed 2020-10-21 21:39 UTC
 * 
 */

#ifndef VIDEO_H
#define VIDEO_H

#include <Magick++.h>
#include <opencv2/opencv.hpp>

class Video {
public:
  Video(const std::string& video_n, const std::string& gif_n)
    : video_name(video_n),
      output_gif_name(gif_n) {}

  ~Video() {}

  Video(const Video& ) = delete;
  Video& operator=( const Video& ) = delete;

  void create_gif();

private:
  void extract_frames();
  static inline Magick::Image mat_to_magick(cv::Mat& src);

private:
  std::string video_name;
  std::string output_gif_name;
  std::vector<cv::Mat> frames;
  std::vector<Magick::Image> magick_frames;
};

#endif

