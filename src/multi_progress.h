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
#include <memory>
#include <ostream>
#include <tuple>
#include <type_traits>
#include <atomic>
#include <mutex>
#include <utility>
#include <vector>

static inline void move_up(int lines) { std::cout << "\033[" << lines << "A"; }
static inline void move_down(int lines) { std::cout << "\033[" << lines << "B"; }
static inline void move_right(int cols) { std::cout << "\033[" << cols << "C"; }
static inline void move_left(int cols) { std::cout << "\033[" << cols << "D"; }

template<typename ProgressBar>
class multi_progress
{
public:
  template<typename... ProgressBars>
  multi_progress(ProgressBars&... bars) {
    pbars = {bars...}; 
  }
  multi_progress() { }

  template<typename T>
  void add_bar(std::unique_ptr<T>&& bar) {
    pbars.emplace_back(std::move(bar));
  }

  template<size_t index>
  void update(float val, std::ostream& os = std::cout) {
    pbars[index].get()->set_progress(val);
    write_progress(os);
  }

  void update(float val, size_t index, std::ostream& os = std::cout) {
    pbars[index].get()->set_progress(val);
    write_progress(os);
  }

  template<size_t index>
  void enable() {
    std::unique_lock lock{mpb_mutex};
    pbars[index].get()->set_enable();
  }

  void enable(size_t index) {
    std::unique_lock lock{mpb_mutex};
    pbars[index].get()->set_enable();
  }

  template<size_t index>
  void disable() {
    std::unique_lock lock{mpb_mutex};
    pbars[index].get()->set_disable();
  }

  void disable(size_t index) {
    std::unique_lock lock{mpb_mutex};
    pbars[index].get()->set_disable();
  }

  void set_prefix(size_t index, const std::string& prefix) {
    std::unique_lock lock{mpb_mutex};
    pbars[index].get()->set_prefix_text(prefix);
  }

  void write_progress(std::ostream& os = std::cout) {
    std::unique_lock lock{mpb_mutex};

    for (auto& bar : pbars) {
      if (bar.get()->running() || bar.get()->complete()) {
        bar.get()->write_progress();
          os<<"\n";
      }
    }

    int count = 0;

    for (auto& bar : pbars) {
      if (bar.get()->running() || bar.get()->complete()) ++count;
    }
    move_up(count);

    if (!started) {
      started = true;
    }
  }
private:
  std::atomic<bool> started{false};
  std::mutex mpb_mutex;
  std::vector<std::unique_ptr<ProgressBar>> pbars;
};

#endif
