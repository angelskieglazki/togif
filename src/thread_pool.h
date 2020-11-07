/**
 * \file thread_pool.h
 * \author max
 * Create on Sat 2020-11-07 11:31 UTC
 * 
 */

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include <type_traits>
#include <vector>
#include <queue>
#include <memory>

#include <future>
#include <functional>
#include <stdexcept>

constexpr size_t max_que_size = 100;

class thread_pool {
public:
  thread_pool(size_t threads);

  template<class T, class... Args>
  decltype(auto) add_to_thread_pool(T&& t, Args&&... args);

  ~thread_pool();

private:
  void new_worker();

private:
  std::mutex q_mutex;
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  std::condition_variable tasks_full_flag;
  std::condition_variable cv;
  bool stop = false;
};


thread_pool::thread_pool(size_t threads) {
  workers.reserve(threads);
  for (size_t i = 0; i < threads; ++i) {
    new_worker();
  }
}


thread_pool::~thread_pool() {
  {
    std::unique_lock<std::mutex> lock{ q_mutex };
    stop = true;
  }

  cv.notify_all();

  for (auto& worker : workers) {
    worker.join();
  }
}


template<class T, class... Args>
decltype(auto) thread_pool::add_to_thread_pool(T&& t, Args&&... args) {
  using ret_type = std::invoke_result_t<T, Args...>;

  auto task = std::make_shared<std::packaged_task<ret_type()>> (
        std::bind(std::forward<T>(t), std::forward<Args>(args)...)
      );

  std::future<ret_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(q_mutex);

    if (stop) {
      throw std::runtime_error("Add task on stopped thread pool!");
    }

    if (tasks.size() >= max_que_size) {
      tasks_full_flag.wait(lock, [&](){
            return tasks.size() < max_que_size;
          });
    }

    tasks.emplace([task]() { return (*task)(); });
  }

  cv.notify_one();

  return res;
}


void thread_pool::new_worker() {
  auto& f = tasks_full_flag;

  workers.emplace_back(
      [this, &f] {
        for (;;) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(this->q_mutex);
            this->cv.wait(lock, [&] { return this->stop || !this->tasks.empty(); });

            if (this->stop && this->tasks.empty()) return;

            task = std::move(this->tasks.front());
            this->tasks.pop();
            f.notify_one();
          }

          task();
        }
      });
}
#endif

