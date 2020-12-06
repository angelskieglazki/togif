//g++ -std=c++17 -o test_pb test_progress_bar.cpp -lpthread
#include "progress_bar.h"
#include <thread>

int main() {

  progress_bar bar;
  bar.set_bar_width(50);
  bar.fill_bar_progress_with("=");
  bar.fill_bar_remainder_with(" ");

  for (size_t i = 1; i <= 100; ++i) {
    bar.update(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }


}
