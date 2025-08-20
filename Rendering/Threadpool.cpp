#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });

                    if (stop && tasks.empty())
                        return;

                    task = std::move(tasks.front());
                    tasks.pop();
                    active_tasks++;
                }

                task();

                {
                    std::lock_guard<std::mutex> lock(wait_mutex);
                    active_tasks--;
                }
                wait_condition.notify_one();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable())
            worker.join();
    }
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(wait_mutex);
    wait_condition.wait(lock, [this]() {
        return tasks.empty() && active_tasks == 0;
    });
}
