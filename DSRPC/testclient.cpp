//#include <iostream>
//#include <memory>
//#include <string>
//
//#include "../DSCommunication/AsyncTcpClient.h"
//
//#include "RPCPacket.pb.h"
//#include "RPCPacketFactory.h"
//
//using DSFramework::DSCommunication::AsyncTcpClient;
//using DSFramework::DSRPC::Packet::RPCPacket;
//using DSFramework::DSRPC::RPCPacketFactory;
//
//bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
//{
//	if (!packet || !data || !size) {
//		return false;
//	}
//
//	*size = packet->ByteSizeLong();
//
//	*data = new char[*size];
//	if (!*data) {
//		return false;
//	}
//
//	if (*size > INT32_MAX)
//	{
//		delete[] * data;
//		*data = nullptr;
//		*size = 0;
//		return false;
//	}
//
//	if (!packet->SerializeToArray(const_cast<char*>(*data), (int)*size)) {
//		delete[] * data;
//		*data = nullptr;
//		*size = 0;
//		return false;
//	}
//	return true;
//}
//
//int test() 
//{
//	std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
//	packet->set_service("test");
//	packet->set_from("client");
//	packet->set_to("server");
//	packet->set_type(DSFramework::DSRPC::Packet::RPCPacketType::TASK_REQUEST);
//
//	const char* data = nullptr;
//	size_t size = 0;
//
//	Serialize(packet, &data, &size);
//
//	AsyncTcpClient client;
//	client.Connect("127.0.0.1", 9000);
//	client.Send(data, static_cast<int>(size));
//	system("pause");
//	client.Disconnect();
//	return 0;
//}
