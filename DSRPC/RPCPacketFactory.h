#pragma once
#include <boost/date_time/posix_time/posix_time.hpp>

namespace DSFramework {
	namespace DSRPC {
		namespace Packet {
			class RPCPacketFactory
			{
			private:
				inline std::string GetCurrentTime() {
					return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
				}
			public:

			};
		}
	}
}
