#include "../DSCommunication/AsyncTcpServer.h"
#include "../DSCommunication/SessionManager.h"
#include "../DSCommunication/EventHandler.h"

#include "RPCServerStub.h"

using DSFramework::DSRPC::RPCServerStub;

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::ICloseEventHandler;
using DSFramework::DSCommunication::IDataEventHandler;
using DSFramework::DSCommunication::IConnectEventHandler;
using DSFramework::DSCommunication::AsyncTcpServer;
using DSFramework::DSCommunication::SessionManager;


int main()
{
	std::shared_ptr<RPCServerStub> rpcServerStub = std::make_shared<RPCServerStub>();
	std::shared_ptr<SessionManager> sessionManager = std::make_shared<SessionManager>(100);
	AsyncTcpServer server(9000);
	server.AddConnectEventHandler(std::static_pointer_cast<IConnectEventHandler>(sessionManager));
	server.AddCloseEventHandler(std::static_pointer_cast<ICloseEventHandler>(sessionManager));
	server.Start();
	return 0;
}
