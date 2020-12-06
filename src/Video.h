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
#include "progress_bar.h"
#include "multi_progress.h"

class Video {
public:
  Video(size_t _idx,
                const std::string& video_n,
                 unsigned short frame_h,
                 unsigned short frame_w,
                 unsigned short skip_f,
                 size_t gif_q
    ) : idx(_idx),
        video_name(video_n),
        output_gif_name(video_n+".gif"),
        frame_height(frame_h),
        frame_width(frame_w),
        skip_frame_count(skip_f),
        gif_quality(gif_q)
 {
/*   bar = new progress_bar();
   bar->set_bar_width(70);
   bar->set_status_text(video_n);
   bar->fill_bar_progress_with("=");
   bar->fill_bar_remainder_with(" ");
  */
 }

  ~Video() {/* delete bar; */}

  Video& operator=( const Video& ) = delete;

  const std::string& name() { return video_name; }
  void set_multiprogress_bar(multi_progress<progress_bar>* m) {
    multiprogress_bar = m;
  }

  int create_gif();
  friend std::ostream& operator<<(std::ostream& os, const Video& dt);
private:
  void extract_frames();
  Magick::Image mat_to_magick(cv::Mat& src);

private:
  size_t idx = 0;
  std::string video_name = "";
  std::string output_gif_name = "";
  unsigned short frame_height = 240;
  unsigned short frame_width = 320;
  unsigned short skip_frame_count = 10;
  size_t gif_quality = 75;
  std::vector<cv::Mat> frames;
  std::vector<Magick::Image> magick_frames;
  multi_progress<progress_bar>* multiprogress_bar = nullptr;
//  progress_bar* bar;
};


#endif

