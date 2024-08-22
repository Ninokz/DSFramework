#include <iostream>
#include <memory>
#include <string>

#include "../DSCommunication/AsyncTcpServer.h"
#include "../DSCommunication/SessionManager.h"
#include "../DSCommunication/EventHandler.h"
#include "../DSCommunication/ThreadPool.h"

#include "RPCServerStub.h"
#include "RPCPacketFactory.h"
#include "RPCEventHandler.h"
#include "RPCPacketManager.h"
#include "ServiceProcedure.h"

using DSFramework::DSRPC::RPCServerStub;
using DSFramework::DSRPC::RPCPacketFactory;
using DSFramework::DSRPC::RPCEventHandler;
using DSFramework::DSRPC::RPCPacketManager;

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::ICloseEventHandler;
using DSFramework::DSCommunication::IDataEventHandler;
using DSFramework::DSCommunication::IConnectEventHandler;
using DSFramework::DSCommunication::AsyncTcpServer;
using DSFramework::DSCommunication::SessionManager;
using DSFramework::DSComponent::ThreadPool;

int main()
{
	//std::shared_ptr<RPCServerStub> rpcServerStub = std::make_shared<RPCServerStub>();
	//std::shared_ptr<RPCPacketManager> rpcPacketManager = std::make_shared<RPCPacketManager>();
	////rpcServerStub->AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(rpcPacketManager));
	////rpcServerStub->AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(rpcPacketManager));
	////rpcServerStub->AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(rpcPacketManager));


	//std::shared_ptr<SessionManager> sessionManager = std::make_shared<SessionManager>(100);
	//AsyncTcpServer server(9000);
	//server.AddConnectEventHandler(std::static_pointer_cast<IConnectEventHandler>(sessionManager));
	//server.AddCloseEventHandler(std::static_pointer_cast<ICloseEventHandler>(sessionManager));
	//server.AddDataEventHandler(std::static_pointer_cast<IDataEventHandler>(rpcServerStub));
	//server.Start();



	return 0;
}
