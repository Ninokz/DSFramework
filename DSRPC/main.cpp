#include <iostream>
#include <memory>
#include <string>

#include "RPCServer.h"
#include "RPCClient.h"
#include "RPCPacket.pb.h"

using DSFramework::DSRPC::RPCServer;
using DSFramework::DSRPC::RPCProcessor;
using DSFramework::DSRPC::RPCClient;

using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::Packet::HelloWorldServiceParameters;
using DSFramework::DSRPC::Packet::HelloWorldServiceResult;

class TestHelloWorldService
{
public:
	std::string serviceName = "HelloWorldService";
public:
	bool ParametersCheck(std::shared_ptr<RPCPacket> packet)
	{
		if (!packet->has_parameters()) {
			return false;
		}
		HelloWorldServiceParameters parameters;
		if (packet->parameters().UnpackTo(&parameters))
		{
			return true;
		}
		return false;
	}

	void Execute(std::shared_ptr<RPCPacket> packet)
	{
		HelloWorldServiceParameters parameters;
		packet->parameters().UnpackTo(&parameters);
		auto name = parameters.name();
		auto msg = parameters.message();
		LOG_DEBUG_CONSOLE("HelloWorldService: " + name + " " + msg);
		HelloWorldServiceResult result;
		result.set_message("Hello " + name + " " + msg);
		google::protobuf::Any result_any;
		result_any.PackFrom(result);
		packet->mutable_result()->CopyFrom(result_any);
	}
public:
	static std::shared_ptr<RPCPacket> CreateHelloWorldServiceRequestPacket(std::string name, std::string msg, std::string from, std::string to)
	{
		HelloWorldServiceParameters parameters;
		parameters.set_name(name);
		parameters.set_message(msg);
		std::shared_ptr<RPCPacket> packet;
		packet->set_service("HelloWorldService");
		google::protobuf::Any parameters_any;
		parameters_any.PackFrom(parameters);
		packet->mutable_parameters()->CopyFrom(parameters_any);

		packet->set_from(from);
		packet->set_to(to);

		packet->set_type(DSFramework::DSRPC::Packet::TASK_REQUEST);
		return packet;
	}
};


int main()
{
	TestHelloWorldService testService;
	RPCServer server("serverID", "serverName", 9000, 100, 100, 100);
	server.AddService(testService.serviceName, std::bind(&TestHelloWorldService::ParametersCheck, &testService, std::placeholders::_1), std::bind(&TestHelloWorldService::Execute, &testService, std::placeholders::_1));
	server.Start();
	return 0;
}
