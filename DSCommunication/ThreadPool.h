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

			/// �漰ת��,ģ�������۵�������
			/// && ��ʾ��ֵ����, T&&��ʾ��������, �������õ������ƶϹ�����:
			/// 1. ��������ʵ������ֵ, T���ƶ�Ϊ��ֵ����
			/// 2. ��������ʵ������ֵ, T���ƶ�Ϊ��ֵ����
			/// 3. ��������ʵ������ֵ����, T���ƶ�Ϊ��ֵ����
			/// 
			/// & ��ֵ����
			/// 1. ��������ʵ������ֵ, T���ƶ�Ϊ��ֵ����
			/// 2. ��������ʵ������ֵ, T���ƶ�Ϊ��ֵ����
			/// 3. ��������ʵ������ֵ����, T���ƶ�Ϊ��ֵ����
			/// 
			/// std::forward �����������ù�������ֵ��ת��Ϊ��ֵ������ʹ��ʱ��ƥ�������
			template<class F, class... Args>
			auto Commit(F&& f, Args&&... args) ->
				/// ����ֵ�����ƶ�, ԭ��ת��f��args,��Ϊf��������ֵ����, argsҲ��������ֵ
				std::future<decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
				using ReturnType = decltype(std::forward<F>(f)(std::forward<Args>(args)...));

				if (m_stop.load())
					return std::future<ReturnType>();

				/// ����һ������
				auto task = std::make_shared<std::packaged_task<ReturnType()>>(
					std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);
				
				/// ��ȡ�����future
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