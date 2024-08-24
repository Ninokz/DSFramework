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
		void TestRPCServer()
		{
			TestHelloWorldService testService;
			RPCServer server("serverID", "serverName", 9000, 100, 100, 100);
			server.AddService(testService.serviceName, std::bind(&TestHelloWorldService::ParametersCheck, &testService, std::placeholders::_1), std::bind(&TestHelloWorldService::Execute, &testService, std::placeholders::_1));
			server.Start();
		}
	}
}