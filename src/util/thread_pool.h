#ifndef gnat_util_thread_pool_H
#define gnat_util_thread_pool_H

#include "gnat.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace gnat {

/** A simple and very limited interface that allows you to make a pool of threads
 *	do work from a queue of jobs */
class ThreadPool {
 public:
	/** Constructor
	 *	@param numThreads How many threads in the pool */
	ThreadPool(int num_threads);
	~ThreadPool();

	/** A single unit of work for a thread to do */
	class Job {
	 public:
		virtual void Execute() = 0;
	};

	/** Add a job to the queue
	 *		@param job The job
	 *		@remarks Adding a NULL job will cause the worker that gets
	 *			that job to terminate */
	void AddJob(Job* job);

	/** Start the workers working... */
	void StartWorkers();

	/** Wait until all of the workers are done */
	void WaitForWorkers();

	/** Function used by worker threads */
	static void WorkerThread(ThreadPool* pool);

 protected:

	// mutex that protects the job queue
	boost::mutex job_mutex_;

	// the queue of pending jobs
	std::deque<Job*> jobs_;

	// counters of pending and active jobs
	unsigned int active_jobs_;
	unsigned int pending_jobs_;

	// how many threads in the pool (fixed atm, may be variable later...)
	unsigned int num_threads_;

	// used to signal workers that there's work to be done
	boost::condition_variable_any job_signal_;

	// used to signal that the jobs are done
	boost::condition_variable_any job_done_signal_;

	// the pool of threads itself
	boost::thread_group thread_pool_;
};

}  // namespace gnat

#endif  // gnat_util_thread_pool_H
