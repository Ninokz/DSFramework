#pragma once
#include <string>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace DSFramework {
	namespace DSRPC {
		class RPCTask {
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
		};
	}
}