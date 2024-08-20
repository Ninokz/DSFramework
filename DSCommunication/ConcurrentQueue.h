#pragma once
#include <memory>
#include <mutex>
#include <condition_variable>

namespace DSFramework {
	namespace DSComponent {
		template <typename T>
		class ConcurrentQueue
		{
		private:
			struct Node {
				std::shared_ptr<T> data;
				std::unique_ptr<Node> next;
			};
			std::mutex m_headMutex;
			std::mutex m_tailMutex;
			std::unique_ptr<Node> m_head;
			Node* m_tail;
			std::condition_variable m_dataCondi;

			std::atomic<int> m_count;
		private:
			Node* get_tail()
			{
				std::lock_guard<std::mutex> tail_lock(m_tailMutex);
				return m_tail;
			}

			std::unique_ptr<Node> pop_head()
			{
				std::unique_ptr<Node> old_head = std::move(m_head);
				m_head = std::move(old_head->next);
				return old_head;
			}

			std::unique_ptr<Node> wait_pop_head()
			{
				std::unique_lock<std::mutex> head_lock(m_headMutex);
				m_dataCondi.wait(head_lock, [&] { return m_head.get() != get_tail(); });
				return pop_head();
			}

		public:
			ConcurrentQueue():m_head(new Node), m_tail(m_head.get()){}
			ConcurrentQueue(const ConcurrentQueue& other) = delete;
			ConcurrentQueue& operator=(const ConcurrentQueue& other) = delete;

			std::shared_ptr<T> WaitPop()
			{
				std::unique_ptr<Node> const old_head = wait_pop_head();
				m_count.fetch_sub(1);
				return old_head->data;
			}

			bool empty()
			{
				std::lock_guard<std::mutex> head_lock(m_headMutex);
				return (m_head.get() == get_tail());
			}

			inline int size()
			{
				return m_count.load();
			}

			void Push(T newValue) 
			{
				std::shared_ptr<T> new_data(std::make_shared<T>(std::move(newValue)));
				std::unique_ptr<Node> p(new Node);
				Node* const new_tail = p.get();
				{
					std::lock_guard<std::mutex> tail_lock(m_tailMutex);
					m_tail->data = new_data;
					m_tail->next = std::move(p);
					m_tail = new_tail;
				}
				m_count.fetch_add(1);
				m_dataCondi.notify_one();
			}
		};
	}
}
