#pragma once
#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <functional>
#include <type_traits>

#include "Logger.h"
#include "Singleton.h"


namespace DSFramework {
	namespace DSComponent {
		class ThreadPool : public Singleton<ThreadPool>
		{
			friend class Singleton<ThreadPool>;
			using Task = std::packaged_task<void()>;
		private:
			std::atomic_int m_threadNum;
			std::queue<Task> m_tasks;
			std::vector<std::thread> m_pool;
			std::atomic_bool m_stop;

			std::mutex  m_cv_mt;
			std::condition_variable m_cv_lock;
		private:
			ThreadPool(int poolSize = std::thread::hardware_concurrency());
			void Start();
			void Stop();

			ThreadPool(const ThreadPool&) = delete;
			ThreadPool& operator=(const ThreadPool&) = delete;
			ThreadPool(ThreadPool&&) = delete;
		public:
			virtual ~ThreadPool();

			/// 涉及转发,模板引用折叠的问题
			/// && 表示右值引用, T&&表示万能引用, 万能引用的类型推断规则是:
			/// 1. 如果传入的实参是左值, T被推断为左值引用
			/// 2. 如果传入的实参是右值, T被推断为右值引用
			/// 3. 如果传入的实参是右值引用, T被推断为右值引用
			/// 
			/// & 右值引用
			/// 1. 如果传入的实参是左值, T被推断为左值引用
			/// 2. 如果传入的实参是右值, T被推断为左值引用
			/// 3. 如果传入的实参是右值引用, T被推断为右值引用
			/// 
			/// std::forward 避免了在引用过程中右值被转换为左值导致在使用时不匹配的问题
			template<class F, class... Args>
			auto Commit(F&& f, Args&&... args) ->
				/// 返回值类型推断, 原样转发f和args,因为f可能是右值引用, args也可能是右值
				std::future<decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
				using ReturnType = decltype(std::forward<F>(f)(std::forward<Args>(args)...));

				if (m_stop.load())
					return std::future<ReturnType>();

				/// 创建一个任务
				auto task = std::make_shared<std::packaged_task<ReturnType()>>(
					std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);
				
				/// 获取任务的future
				std::future<ReturnType> res = task->get_future();
				{
					std::lock_guard<std::mutex> lock(m_cv_mt);
					m_tasks.emplace([task]() { (*task)(); });
				}
				m_cv_lock.notify_one();
				return res;
			}
		
			int GetIdleThreadCount() const { return m_threadNum.load(); }
		};
	}
}