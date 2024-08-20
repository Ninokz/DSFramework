#pragma once
#include <mutex>
#include <unordered_map>
#include <shared_mutex>

#include "RPCTaskWrapper.h"

namespace DSFramework {
	namespace DSRPC {
		template<class TTask>
		class RPCTaskManager
		{
			using TASKID = std::string;
		private:
			std::shared_mutex m_mutex;
			std::unordered_map<TASKID, RPCTaskWrapper<TTask>> m_tasks;
		public:
			
		};
	}
}