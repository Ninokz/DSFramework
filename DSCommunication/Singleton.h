#pragma once
#include <memory>
#include <mutex>

namespace DSFramework {
	namespace DSComponent {
		template <typename T>
		class Singleton {
		protected:
			Singleton() = default;
			Singleton(const Singleton&) = delete;
			Singleton& operator=(const Singleton<T>& st) = delete;
			Singleton(Singleton&&) = delete;

			static std::shared_ptr<T> m_instance;
		public:
			virtual ~Singleton() = default;

			static std::shared_ptr<T> GetInstance() 
			{
				static std::once_flag flag;
				std::call_once(flag, [&]() {
					m_instance = std::shared_ptr<T>(new T);
				});
				return m_instance;
			}
		};
		template <typename T>
		std::shared_ptr<T> Singleton<T>::m_instance = nullptr;
	}
}