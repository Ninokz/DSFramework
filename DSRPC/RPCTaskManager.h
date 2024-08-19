#pragma once
#include <mutex>
#include <unordered_map>
#include <shared_mutex>

#include "RPCTask.h"

namespace DSFramework {
	namespace DSRPC {
		template<class TTask>
		class RPCTaskManager
		{
			using TASKID = std::string;
		private:
			std::shared_mutex m_mutex;
			std::unordered_map<TASKID, RPCTask> m_tasks;
		public:
			
		};
	}
}