/**
 * \file GifCreator.cpp
 * \author max
 * Create on Sun 2020-12-06 12:50 UTC
 * 
 */

#include "GifCreator.h"
#include "thread_pool.h"

void GifCreator::add_video(std::unique_ptr<Video>&& video) {
  auto bar = std::make_unique<progress_bar>();
  bar->set_status_text(video->name());
  multibars.add_bar(std::move(bar));
  video->set_multiprogress_bar(&multibars);
  videos.emplace_back(std::move(video)); 
}


void GifCreator::start_gif_creating() {
  thread_pool pool(2);
  std::vector< std::future<int> > results;

  for (auto& v : videos) {
    results.emplace_back(
          pool.add_to_thread_pool([&] {
              return v->create_gif();
            })
        );
  }


}

