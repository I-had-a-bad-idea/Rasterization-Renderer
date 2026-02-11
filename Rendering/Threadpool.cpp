#include "ThreadPool.h"

// Simple thread pool implementation
ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task; // Task to execute
                // Wait for a task or stop signal
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });
                    // If stopping and no tasks left, exit thread
                    if (stop && tasks.empty())
                        return;
                    // Get next task
                    task = std::move(tasks.front());
                    tasks.pop();
                    active_tasks++;
                }
                // Execute the task
                task();
                // Task finished, update active task count
                {
                    std::lock_guard<std::mutex> lock(wait_mutex);
                    active_tasks--;
                }
                // Notify waiting thread if all tasks are done
                wait_condition.notify_one();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    // Signal all threads to stop
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop = true;
    }
    // Notify all threads to wake up and exit
    condition.notify_all();
    // Join all worker threads
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
