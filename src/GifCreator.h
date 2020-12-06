/**
 * \file GifCreator.h
 * \author max
 * Create on Sun 2020-12-06 12:50 UTC
 * 
 */

#ifndef GIFCREATOR_H
#define GIFCREATOR_H

#include <memory>

#include "Video.h"
#include "multi_progress.h"
#include "progress_bar.h"

class GifCreator {
public:
  void add_video(std::unique_ptr<Video>&& video);
  void start_gif_creating();
private:
  std::vector<std::unique_ptr<Video>> videos;
  multi_progress<progress_bar> multibars;
};

#endif

