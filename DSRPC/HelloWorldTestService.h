#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "RPCServer.h"
#include "RPCClient.h"
#include "RPCPacket.pb.h"

#include "google/protobuf/any.h"

using DSFramework::DSRPC::RPCServer;
using DSFramework::DSRPC::RPCProcessor;
using DSFramework::DSRPC::RPCClient;

using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::Packet::HelloWorldServiceParameters;
using DSFramework::DSRPC::Packet::HelloWorldServiceResult;


namespace DSFramework {
	namespace DSTest {
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

				google::protobuf::Any* result_any = packet->mutable_result();
				result_any->PackFrom(result);
			}
		public:
			static std::shared_ptr<RPCPacket> CreateHelloWorldServiceRequestPacket(std::string name, std::string msg, std::string from, std::string to)
			{
				std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
				packet->set_service("HelloWorldService");

				HelloWorldServiceParameters parameters;
				parameters.set_name(name);
				parameters.set_message(msg);
				google::protobuf::Any* parameters_any = packet->mutable_parameters();
				parameters_any->PackFrom(parameters);

				packet->set_from(from);
				packet->set_to(to);
				packet->set_type(DSFramework::DSRPC::Packet::TASK_REQUEST);
				return packet;
			}
		};

	}
}