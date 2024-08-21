#pragma once
#include <mutex>
#include <unordered_map>
#include <shared_mutex>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace DSFramework {
	namespace DSRPC {
		class RPCTaskManager
		{
		public:

		public:
			RPCTaskManager();
			virtual ~RPCTaskManager();
		};
	}
}