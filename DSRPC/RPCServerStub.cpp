#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub()
		{
		}

		RPCServerStub::~RPCServerStub()
		{
		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
		}
	}
}