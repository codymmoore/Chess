#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

namespace util
{
	class ThreadPool
	{
	public:
		/**
		 * Get singleton instance of ThreadPool.
		 *
		 * \return singleton instance of ThreadPool
		 */
		static ThreadPool& getInstance()
		{
			static ThreadPool threadPool;
			return threadPool;
		}

		/**
		 * Submit a function to be executed asynchronously.
		 *
		 * \param func the function to be executed
		 * \param ...args the function arguments
		 * \return future containing the result of the function
		 */
		template <typename Func, typename... Args>
		auto submit(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
		{
			if (_stop.load())
			{
				throw std::exception("Threadpool has stopped execution");
			}

			using ReturnType = decltype(func(args...));

			#ifdef TEST
			return std::async(std::launch::async, std::forward<Func>(func), std::forward<Args>(args)...);
			#endif

			auto task = std::make_shared<std::packaged_task<ReturnType()>>(
				[func = std::forward<Func>(func), ... args = std::forward<Args>(args)]() mutable {
				return func(std::forward<Args>(args)...);
			}
			);
			std::future<ReturnType> result = task->get_future();
			{
				std::lock_guard<std::mutex> lock(_tasksMutex);
				_tasks.emplace([task]() { (*task)(); });
			}
			_conditionVariable.notify_one();

			return result;
		}

	private:
		/**
		 * Create a new instace of ThreadPool.
		 *
		 * \param threadCount the number of threads used to execute tasks
		 */
		ThreadPool(const size_t threadCount = std::thread::hardware_concurrency())
		{
			for (size_t i = 0; i < threadCount; i++)
			{
				_workers.emplace_back(&ThreadPool::workerThread, this);
			}
		}

		/**
		 * Destruct ThreadPool instance.
		 */
		~ThreadPool()
		{
			_stop.store(true);
			_conditionVariable.notify_all();
			for (std::thread& worker : _workers)
			{
				if (worker.joinable())
				{
					worker.join();
				}
			}
		}

		/**
		 * Executed by each worker thread.
		 */
		void workerThread()
		{
			while (true)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(_tasksMutex);
					_conditionVariable.wait(lock, [&]() {
						return _stop.load() || !_tasks.empty();
					});

					if (_stop.load() && _tasks.empty())
					{
						return;
					}

					task = std::move(_tasks.front());
					_tasks.pop();
				}
				task();
			}
		}

		std::vector<std::thread> _workers;

		std::queue<std::function<void()>> _tasks;

		std::mutex _tasksMutex;

		std::condition_variable _conditionVariable;

		std::atomic<bool> _stop;
	};


}
