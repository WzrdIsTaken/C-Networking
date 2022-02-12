// Created by Ben on 24/01/2022

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

// Handles everything to do with multithreading

namespace bcw
{
  class ThreadHandler // Resources used can be found on this thread: https://bit.ly/359aUyC
  {
   public:
    ThreadHandler();
    ~ThreadHandler();

    ThreadHandler(const ThreadHandler&) = delete;
    ThreadHandler& operator=(const ThreadHandler&) = delete;

    // Adds a job to the queue to be processed
    [[maybe_unused]] void scheduleJob(const std::function<void()>& job);
    // Terminates all threads in the pool
    [[maybe_unused]] void destroyThreads();

    // Number of all the threads in the pool
    [[maybe_unused]] [[nodiscard]] int getSize() const;
    // Number of jobs in the queue
    [[maybe_unused]] [[nodiscard]] int getNumberOfJobs() const;

   private:
    // An infinite loop ran on a thread that will grab an item off the jobs_queue and execute it
    void handleJob();

    // All the threads
    std::vector<std::thread> thread_pool;
    // Stores all the jobs waiting to be run
    std::queue<std::function<void()>> jobs_queue;
    // To prevent nasty threading related disasters
    std::mutex queue_mutex;
    // Used in conjunction with the mutex to allow threads to talk with each other
    std::condition_variable data_condition;
    // Used to stop all thread execution
    std::atomic<bool> terminate_pool = false;
  };
} // namespace bcw

// To schedule a job:
// std::function<void()> func = [this] { function(); }; (member functions - https://bit.ly/3qSMgLc)
// (might be able to do it a different way with non-member functions I haven't tested yet)
// bcw::Driver::getThreadHandler()->scheduleJob(func);
