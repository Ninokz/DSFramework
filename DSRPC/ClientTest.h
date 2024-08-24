#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "RPCServer.h"
#include "RPCClient.h"
#include "RPCPacket.pb.h"
#include "HelloWorldTestService.h"

#include "google/protobuf/any.h"

using DSFramework::DSRPC::RPCServer;
using DSFramework::DSRPC::RPCProcessor;
using DSFramework::DSRPC::RPCClient;

using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::Packet::HelloWorldServiceParameters;
using DSFramework::DSRPC::Packet::HelloWorldServiceResult;

namespace DSFramework {
	namespace DSTest {
		void TestRPCClient()
		{
			RPCClient client("clientid", "clientName");
			client.RemoteCall("127.0.0.1", 9000, TestHelloWorldService::CreateHelloWorldServiceRequestPacket("Alice", "World", "Client", "Server"), 20);
		}
	}
}
