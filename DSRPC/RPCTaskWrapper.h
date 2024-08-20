#pragma once
#include <string>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace DSFramework {
	namespace DSRPC {
		template<class TTask>
		class RPCTaskWrapper
		{
		public:
			enum class RPCTaskStatus
			{
				TaskWaiting,
				TaskCommited,
				TaskCompleted,
				TaskFailed
			};
		public:
			std::string m_taskID;
			boost::posix_time::ptime m_createdTime;
			boost::posix_time::ptime m_commitedTime;
			boost::posix_time::ptime m_completedTime;
			RPCTaskStatus m_status;
			std::string m_submitterID;	// SessionID

			std::shared_ptr<TTask> m_task;
		public:
			RPCTaskWrapper(const std::string& submitterID) :
				m_taskID(boost::uuids::to_string(boost::uuids::random_generator()())),
				m_createdTime(boost::posix_time::second_clock::local_time()), 
				m_status(RPCTaskStatus::TaskWaiting),
				m_submitterID(submitterID)
			{}

			virtual ~RPCTaskWrapper() {}
		};
	}
}