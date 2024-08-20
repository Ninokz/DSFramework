#include "RPCTaskManager.h"


namespace DSFramework {
	namespace DSRPC {
		RPCTaskManager::RPCTaskManager()
		{

		}

		RPCTaskManager::~RPCTaskManager()
		{

		}

		bool RPCTaskManager::AddTask(TASKID& taskId, std::shared_ptr<RPCPacket> task)
		{
			std::unique_lock<std::shared_mutex> lock(m_taskMutex);
			if (this->m_taskMap.find(taskId) == this->m_taskMap.end())
			{
				this->m_taskMap.insert(std::make_pair(taskId, task));
				return true;
			}
			else
				return false;
		}

		void RPCTaskManager::RemoveTask(TASKID& taskId)
		{
			std::unique_lock<std::shared_mutex> lock(m_taskMutex);
			auto it = this->m_taskMap.find(taskId);
			if (it != this->m_taskMap.end())
				this->m_taskMap.erase(it);
		}

		bool RPCTaskManager::IsTaskExist(TASKID& taskId)
		{
			std::shared_lock<std::shared_mutex> lock(m_taskMutex);
			if (this->m_taskMap.find(taskId) != this->m_taskMap.end())
				return true;
			else
				return false;
		}

		bool RPCTaskManager::UpdateTaskStatus(TASKID& taskId, RPCTaskStatus status)
		{
			std::unique_lock<std::shared_mutex> lock(m_taskMutex);
			auto it = this->m_taskMap.find(taskId);
			if (it != this->m_taskMap.end())
			{
				// 获取到任务的共享指针
				std::shared_ptr<RPCPacket> packet = it->second;
				// 更新任务状态
				packet->mutable_task()->set_task_status(status);
				return true;
			}
			else
				return false;
		}
	}
}