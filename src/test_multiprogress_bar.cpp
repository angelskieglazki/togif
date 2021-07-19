//g++ -std=c++17 -o test_mpb test_multiprogress_bar.cpp -lpthread
#include "progress_bar.h"
#include "multi_progress.h"
#include <memory>
#include <thread>
#include <utility>

int main() {

 auto bar1 = std::make_unique<progress_bar>();
 auto bar2 = std::make_unique<progress_bar>();
 auto bar3 = std::make_unique<progress_bar>();
 bar1->set_status_text("1 bar");
 bar2->set_status_text("2 bar");
 bar3->set_status_text("3 bar");
 bar1->set_prefix_text("extracting");
 bar2->set_prefix_text("extracting");
 bar3->set_prefix_text("extracting");

//  multi_progress<progress_bar, 3> bars(bar1, bar2, bar3);
  multi_progress<progress_bar> bars;
  bars.add_bar<progress_bar>(std::move(bar1));
  bars.add_bar<progress_bar>(std::move(bar2));
  bars.add_bar<progress_bar>(std::move(bar3));

  // Job for the first bar
  auto job1 = [&bars]() {
    for (size_t i = 0; i <= 100; ++i) {
      bars.update<0>(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  };

  // Job for the second bar
  auto job2 = [&bars]() {
    for (size_t i = 0; i <= 100; ++i) {
      bars.update<1>(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  };

  // Job for the third bar
  auto job3 = [&bars]() {
    for (size_t i = 0; i <= 100; ++i) {
      bars.update<2>(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
  };

  std::thread first_job(job1);
  std::thread second_job(job2);
  std::thread third_job(job3);

  first_job.join();
  second_job.join();
  third_job.join();



  return 0;

}
