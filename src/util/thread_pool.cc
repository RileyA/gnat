#include "util/thread_pool.h"

namespace gnat {

ThreadPool::ThreadPool(int num_threads)
  : num_threads_(num_threads) {
  for(int i = 0; i < num_threads; ++i)
    thread_pool_.push_back(new std::thread(&WorkerThread, this));
  active_jobs_ = 0;
  pending_jobs_ = 0;
}
//---------------------------------------------------------------------------

ThreadPool::~ThreadPool() {
  // flood the job queue with null jobs...
  for(int i = 0; i < num_threads_ * 2; ++i)
    AddJob(0);

  // start the workers (no workers should be working at this point)
  StartWorkers();

  //  let them all wrap up
  for (int i = 0; i < thread_pool_.size(); ++i) {
    thread_pool_[i]->join();
    delete thread_pool_[i];
  }
}
//---------------------------------------------------------------------------

void ThreadPool::AddJob(Job* job) {
  std::lock_guard<std::mutex> lock(job_mutex_);
  jobs_.push_back(job);
  ++pending_jobs_;
}
//---------------------------------------------------------------------------

void ThreadPool::StartWorkers() {
  std::lock_guard<std::mutex> lock(job_mutex_);
  // just notify one, it'll notify more if need be
  if(!jobs_.empty())
    job_signal_.notify_one();
}
//---------------------------------------------------------------------------

void ThreadPool::WaitForWorkers() {
  // wait for all current jobs to complete
  std::lock_guard<std::mutex> lock(job_mutex_);
  while(pending_jobs_ > 0)
    job_done_signal_.wait(job_mutex_);
}
//---------------------------------------------------------------------------

void ThreadPool::WorkerThread(ThreadPool* pool) {
  Job* assigned = 0;
  bool more_jobs = false;
  bool has_job = false; // poor unemployed worker thread.. ;_;

  // the only way it will terminate is if it receives a null job
  while(true) {
    // lock to access job queue
    {
      std::lock_guard<std::mutex> lock(pool->job_mutex_);

      // skip if this is the first iteration
      if(has_job) {
        // must've just wrapped up a job
        --pool->active_jobs_;
        --pool->pending_jobs_;

        // notify if there are no jobs left
        if(pool->pending_jobs_ == 0)
          pool->job_done_signal_.notify_one();
  
        // if the last job was null, then terminate
        if(!assigned)
          return;
        else
          delete assigned;
      }

      // loop on the condition, to handle spurious wakeups
      // note that this will get skipped if jobs remain in the queue after being awoken
      while(pool->jobs_.empty())
        pool->job_signal_.wait(pool->job_mutex_);

      // if it gets here, there must be a job for it...
      assigned = pool->jobs_.front();
      pool->jobs_.pop_front();
      has_job = true;
      ++pool->active_jobs_;

      more_jobs = !pool->jobs_.empty();
    }

    // now that the mutex is unlocked, if there's still work to be done, notify another thread
    if(more_jobs)
      pool->job_signal_.notify_one();

    // actually do the work...
    if(assigned)
      assigned->Execute();
  }
}
//---------------------------------------------------------------------------

}  // namespace gnat
