#include "RPCTaskManager.h"


namespace DSFramework {
	namespace DSRPC {
		RPCTaskManager::RPCTaskManager()
		{

		}

		RPCTaskManager::~RPCTaskManager()
		{

		}

		DSFramework::DSRPC::RPCTaskManager::TASKID RPCTaskManager::AddTask(std::shared_ptr<RPCPacket> task)
		{
			std::unique_lock<std::shared_mutex> lock(m_taskMutex);
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			TASKID taskId = boost::uuids::to_string(uuid);

			task->mutable_task()->set_task_uid(taskId);
			std::string timestamp = boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
			task->mutable_task()->set_created_time(timestamp);
			task->mutable_task()->set_commited_time(boost::posix_time::not_a_date_time);
			task->mutable_task()->set_completed_time(boost::posix_time::not_a_date_time);
			task->mutable_task()->set_task_status(Packet::RPCTaskStatus::WAITING);

			this->m_taskMap.insert(std::make_pair(taskId, task));
			return taskId;
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