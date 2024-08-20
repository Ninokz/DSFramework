#pragma once
#include <mutex>
#include <unordered_map>
#include <shared_mutex>

#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::Packet::RPCTaskStatus;

namespace DSFramework {
	namespace DSRPC {
		class RPCTaskManager
		{
		public:
			using TASKID = std::string;
			using SESSIONID = std::string;
		private:
			std::shared_mutex m_taskMutex;
			std::unordered_map<TASKID, std::shared_ptr<RPCPacket>> m_taskMap;
		public:
			RPCTaskManager();
			virtual ~RPCTaskManager();

			bool AddTask(TASKID& taskId, std::shared_ptr<RPCPacket> task);
			void RemoveTask(TASKID& taskId);
			bool IsTaskExist(TASKID& taskId);

			bool UpdateTaskStatus(TASKID& taskId, RPCTaskStatus status);
		};
	}
}