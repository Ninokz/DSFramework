#pragma once

#include "RPCTaskWrapper.h"
#include "Dispatcher.h"

namespace DSFramework {
	namespace DSRPC {

		template<class TTask>
		class RequestDispatcher : public Dispatcher<RPCTaskWrapper<TTask>>
		{
		private:

		public:
		
		};
	}
}