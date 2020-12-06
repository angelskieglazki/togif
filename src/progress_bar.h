/**
* \file progress_bar.h
* \author max
* Created on Sat Dec  5 17:09:28 2020
* 
*/

#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <mutex>
#include <atomic>
#include <iostream>
#include <ostream>

class progress_bar {
public:
  void set_progress(float val) {
    std::unique_lock lock{pb_mutex};
    progress = val;
  }

  void set_bar_width(size_t width) {
    std::unique_lock lock{pb_mutex};
    bar_width = width;
  }
  
  void fill_bar_progress_with(const std::string& chars) {
    std::unique_lock lock{pb_mutex};
    fill = chars;
  }

  void fill_bar_remainder_with(const std::string& chars) {
    std::unique_lock lock{pb_mutex};
    reminder = chars;
  }

  void set_status_text(const std::string& val) {
    std::unique_lock lock{pb_mutex};
    status_text = val;
  }

  void set_prefix_text(const std::string& val) {
    std::unique_lock lock{pb_mutex};
    prefix = val;
  }

  void update(float val, std::ostream& os = std::cout) {
    set_progress(val);
    write_progress(os);
  }

  void write_progress(std::ostream& os = std::cout) {
    std::unique_lock lock{pb_mutex};

    if (progress > 100.0f) return;

    os << "\r" <<std::flush;

    os << prefix << " [";

    const auto completed = static_cast<size_t>(
        progress * static_cast<float>(bar_width) / 100.0);
    for (size_t i = 0; i < bar_width; ++i) {
      if (i <= completed) {
        os << fill;
      } else {
        os << reminder;
      }
    }
    
    os << "]";

    os << " " << std::min(static_cast<size_t>(progress),
        size_t(100)) << "%"; 

    os << " " << status_text;
  }
private:
  std::mutex pb_mutex;
  float progress{0.0f};
  size_t bar_width{70};
  std::string fill{"#"};
  std::string reminder{" "};
  std::string status_text{""};
  std::string prefix{""};
};
#endif
