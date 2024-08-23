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

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::ICloseEventHandler;
using DSFramework::DSCommunication::IDataEventHandler;
using DSFramework::DSCommunication::IConnectEventHandler;
using DSFramework::DSCommunication::AsyncTcpServer;
using DSFramework::DSCommunication::SessionManager;
using DSFramework::DSComponent::ThreadPool;

using DSFramework::DSRPC::RPCServerStub;
using DSFramework::DSRPC::RPCPacketFactory;
using DSFramework::DSRPC::RPCEventHandler;
using DSFramework::DSRPC::RPCPacketManager;
using DSFramework::DSRPC::RPCServer;
using DSFramework::DSRPC::RequestDispatcher;
using DSFramework::DSRPC::ResponseDispatcher;

using DSFramework::DSRPC::IDeserializedEventHandler;
using DSFramework::DSRPC::IDeserializedFailedEventHandler;
using DSFramework::DSRPC::IDispatchEventHandler;
using DSFramework::DSRPC::ICommitedEventHandler;
using DSFramework::DSRPC::IServiceEventHandler;
using DSFramework::DSRPC::IProcessedEventHandler;

int main()
{
	RPCEventHandler rpcEventHandler;
	std::shared_ptr<RPCServer> rpcServer = std::make_shared<RPCServer>(rpcEventHandler);

	std::shared_ptr<ResponseDispatcher> responseDispatcher = std::make_shared<ResponseDispatcher>(100);
	std::shared_ptr<RequestDispatcher> requestDispatcher = std::make_shared<RequestDispatcher>(100, rpcEventHandler);
	std::shared_ptr<RPCPacketManager> rpcPacketManager = std::make_shared<RPCPacketManager>();
	std::shared_ptr<RPCServerStub> rpcServerStub = std::make_shared<RPCServerStub>(rpcEventHandler);

	/// IDeserializedEventHandler 事件
	rpcEventHandler.AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(requestDispatcher));
	/// IDispatchEventHandler 事件
	rpcEventHandler.AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(rpcPacketManager));
	/// ICommitedEventHandler 事件
	rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(rpcPacketManager));
	rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(rpcServer));
	/// IServiceEventHandler 事件
	rpcEventHandler.AddServiceEventHandler(std::static_pointer_cast<IServiceEventHandler>(rpcPacketManager));
	/// IProcessedEventHandler 事件
	rpcEventHandler.AddProcessedEventHandler(std::static_pointer_cast<IProcessedEventHandler>(rpcPacketManager));


	//// responseDispatcher 处于调用链末端
	rpcEventHandler.AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(responseDispatcher));
	rpcEventHandler.AddDeserializedFailedEventHandler(std::static_pointer_cast<IDeserializedFailedEventHandler>(responseDispatcher));
	rpcEventHandler.AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(responseDispatcher));
	rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(responseDispatcher));
	rpcEventHandler.AddServiceEventHandler(std::static_pointer_cast<IServiceEventHandler>(responseDispatcher));
	rpcEventHandler.AddProcessedEventHandler(std::static_pointer_cast<IProcessedEventHandler>(responseDispatcher));

	AsyncTcpServer server(9000,100);
	server.AddDataEventHandler(std::static_pointer_cast<IDataEventHandler>(rpcServerStub));
	server.Start();
	return 0;
}
