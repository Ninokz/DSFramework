#pragma once

#include "RPCTask.h"
#include "Dispatcher.h"

namespace DSFramework {
	namespace DSRPC {
		template<class TTask>
		class ResponseDispatcher : public Dispatcher<RPCTask<TTask>>
		{
		private:

		public:

		};
	}
}