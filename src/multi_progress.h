/**
* \file multi_progress.h
* \author max
* Created on Sun Dec  6 13:50:07 2020

* Empty License
* 
*/

#ifndef MULTI_PROGRESS_H
#define MULTI_PROGRESS_H

#include <iostream>
#include <ostream>
#include <type_traits>
#include <atomic>
#include <mutex>
#include <vector>

static inline void move_up(int lines) { std::cout << "\033[" << lines << "A"; }
static inline void move_down(int lines) { std::cout << "\033[" << lines << "B"; }
static inline void move_right(int cols) { std::cout << "\033[" << cols << "C"; }
static inline void move_left(int cols) { std::cout << "\033[" << cols << "D"; }

template<typename ProgressBar, size_t count>
class multi_progress
{
public:
  template<typename... ProgressBars,
           typename = typename std::enable_if_t<sizeof...(ProgressBars) == count>>
  explicit multi_progress(ProgressBars&... bars) {
    pbars = {bars...}; 
  }

  template<size_t index>
  typename std::enable_if_t<(index >=0 && index < count), void>
  update(float val, std::ostream& os = std::cout) {
    pbars[index].get().set_progress(val);
    write_progress(os);
  }

  void write_progress(std::ostream& os = std::cout) {
    std::unique_lock lock{mpb_mutex};
    if (started) {
      for (size_t i = 0; i < count; ++i) {
        os << "\x1b[A";
      }
    }

    for (auto& bar : pbars) {
      bar.get().write_progress();
      os<<"\n";
    }

    if (!started) {
      started = true;
    }
  }
private:
  std::atomic<bool> started{false};
  std::mutex mpb_mutex;
  std::vector<std::reference_wrapper<ProgressBar>> pbars;
};

#endif
