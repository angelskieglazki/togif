#include <iostream>
#include <vector>
#include <chrono>

#include "thread_pool.h"

int main()
{

    thread_pool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 1000; ++i) {
        results.emplace_back(
            pool.add_to_thread_pool([i] {
                std::cout << "hello " << i << std::endl;
//                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

    return 0;
}
