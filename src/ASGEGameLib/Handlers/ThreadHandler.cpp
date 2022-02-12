// Created by Ben on 24/01/2022

#include "../../../include/ASGEGameLib/Handlers/ThreadHandler.hpp"

bcw::ThreadHandler::ThreadHandler()
{
  // I'm not sure if this is bad, using up all the threads, especially if a NetServer is
  // being run with host_is_client = false and on the same machine as the game as then
  // there will be two ThreadHandlers.

  unsigned int max_threads = std::thread::hardware_concurrency();
  for (unsigned int i = 0; i < max_threads; i++)
  {
    thread_pool.emplace_back(&ThreadHandler::handleJob, this); // https://bit.ly/3KEqa73
  }
}

bcw::ThreadHandler::~ThreadHandler()
{
  if (!terminate_pool)
  {
    destroyThreads();
  }
}

void bcw::ThreadHandler::scheduleJob(const std::function<void()>& job)
{
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs_queue.push(job);
  }

  data_condition.notify_one();
}

void bcw::ThreadHandler::destroyThreads()
{
  { // Scopes used because the mutex will automatically unlock once it's out of scope
    std::unique_lock<std::mutex> lock(queue_mutex);
    terminate_pool = true;
  }

  data_condition.notify_all();

  for (std::thread& thread : thread_pool)
  {
    thread.join(); // todo try detach instead + if do this need to clean up ourselves?
  }

  thread_pool.clear();
}

void bcw::ThreadHandler::handleJob()
{
  std::function<void()> job;

  while (true)
  {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      data_condition.wait(lock, [this]() { return !jobs_queue.empty() || terminate_pool; });

      if (terminate_pool && jobs_queue.empty())
      {
        // Required or bad segmentation fault on ThreadHandler destruction
        // because we will be trying to pop an empty queue
        return;
      }

      job = jobs_queue.front();
      jobs_queue.pop();
    }

    job();
  }
}

int bcw::ThreadHandler::getSize() const
{
  return static_cast<int>(thread_pool.size());
}

int bcw::ThreadHandler::getNumberOfJobs() const
{
  return static_cast<int>(jobs_queue.size());
}
