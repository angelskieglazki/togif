/**
 * \file GifCreator.cpp
 * \author max
 * Create on Sun 2020-12-06 12:50 UTC
 * 
 */

#include "GifCreator.h"
#include "thread_pool.h"

void GifCreator::add_video(std::unique_ptr<Video>&& video) {
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

