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
#include "RPCServer.h"
#include "RequestDispatcher.h"
#include "ResponseDispatcher.h"


using DSFramework::DSRPC::RPCServerStub;
using DSFramework::DSRPC::RPCPacketFactory;
using DSFramework::DSRPC::RPCEventHandler;
using DSFramework::DSRPC::RPCPacketManager;
using DSFramework::DSRPC::RPCServer;
using DSFramework::DSRPC::RequestDispatcher;
using DSFramework::DSRPC::ResponseDispatcher;

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
	ResponseDispatcher responseDispatcher(100);
	RPCServer rpcServer(responseDispatcher);

	RPCEventHandler rpcEventHandler;
	std::shared_ptr<RPCPacketManager> rpcPacketManager = std::make_shared<RPCPacketManager>();
	rpcEventHandler.AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(rpcPacketManager));
	rpcEventHandler.AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(rpcPacketManager));
	rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(rpcPacketManager));
	rpcEventHandler.AddProcessedHandler(std::static_pointer_cast<IProcessedHandler>(rpcPacketManager));
	RequestDispatcher requestDispatcher(100, rpcEventHandler, rpcServer);
	std::shared_ptr<RPCServerStub> rpcServerStub = std::make_shared<RPCServerStub>(rpcEventHandler, requestDispatcher);

	std::shared_ptr<SessionManager> sessionManager = std::make_shared<SessionManager>(100);
	AsyncTcpServer server(9000);
	server.AddConnectEventHandler(std::static_pointer_cast<IConnectEventHandler>(sessionManager));
	server.AddCloseEventHandler(std::static_pointer_cast<ICloseEventHandler>(sessionManager));
	server.AddDataEventHandler(std::static_pointer_cast<IDataEventHandler>(rpcServerStub));
	server.Start();
	return 0;
}
