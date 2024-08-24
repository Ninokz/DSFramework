# 🌠DSFramework

## 1. Description 📖

 A C++11 simple remote procedure call  framework based on the `Boost Asio` and `Protobuf` libraries, the core of the framework is based on the master-slave Reactor multi-threading model and the `Boost Asio` asynchronous read/write mechanism.

## 2. Dependency ⚙

- [Boost 1.8.5](https://www.boost.org/users/history/version_1_85_0.html)
- [Google protobuf 4.25.1](https://github.com/protocolbuffers/protobuf/tree/main/src)

## 3. Design 📏

Basic structure of the framework below. 

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSC.drawio.png"></div>

Currently, this framework supports peer-to-peer remote procedure calls, and centralized services to discover or manage service instances are being tested. Next are a few brief descriptions of the functional layers.

### 3.1 Connection  Layer

- **Asio-Connection acceptor**

  use `async_accept` processes connection requests asynchronously, constructs the session and registers it with the Asio IO-Service pool.
  
  1. Receive connection requests asynchronously
  2. Register session to IO Service Pool

### 3.2 Communication Layer

- **Asio IO-Service Pool**

  <div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCIOServicepool.drawio.png"></div>

  An IOServicePool is a pool that manages multiple `io_context` objects. Each `io_context` is handled by a separate thread for distributing I/O operations.

  1. Assignment.
      When an application initiates an asynchronous operation, this operation is submitted to some `io_context`.
  2. Multi-threaded execution.
      Each `io_context` runs in a separate thread. Multiple threads work in parallel and can handle multiple asynchronous operations at the same time.
  3. Task Scheduling and Execution.
      The `io_context` is responsible for scheduling I/O tasks, and threads get tasks from the `io_context` and execute them. When an I/O operation is completed, the callback function of the task will be executed in the corresponding thread.
  4. Loop Processing.
      Threads continue to fetch tasks from the `io_context` and execute them until the `io_context` is stopped.

- **Session Manager**

  A simple session manager is used to manage the session after the connection is established, focusing on the connected and close events.

- **Communication Event Handler**

  Defines an event handling mechanism for handling various events related to network communication, such as connecting, closing connections, and **data reception**.

### 3.3 Stub

- **Serializer / Deserializer**

  Serialized and deserialized data data based on `Protobuf`

- **RPCEventHandler**

  <div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSRPCEventhandler.drawio.png"></div>

  Framework application layer event scheduler, belongs to the core component part, the interaction of the components of this layer through this component decoupled at the same time rely on this component.

  >Note that the priority of each component in responding to an event is important here, and in general the task state switching component should be at the top of the list.
  >
  >`request manager` > `ResponseDispatcher` >`RequestDispatcher` or `worker`

- **Request Dispatcher**

  <div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/RPCDispatcher.drawio.png"></div>

  The Dispatcher implements a generic task scheduler that can be used to manage and schedule task requests from different senders. This single concurrent queue can also be used as a logical layer message queue, ensuring that requests are thread-safe.

  1. Set the maximum queue length, the queue is full then do not accept to enqueue
  2. Bridges the upper level work thread pool with new task requests, coordinating between the two. When the worker thread pool is full, the dispatch operation is temporarily suspended

-  **Response Dispatcher**

  Get response and send it to remote.

### 3.4 Server

Mainly contains a thread pool for executing remote service requests, service name parameters and procedures need to be defined and registered within the component. In this framework, the processor implements this functionality, and the server simply coordinates all of the above modules in one place.

1. Using Thread Pools to Process Service Requests.

### 3.5 Logger

For logging in multi-threaded environments, you can avoid logging blocking the execution of the main thread.

1. Asynchronous output logs to console and quantitative input logs to file to avoid frequent disk reads and writes

## 4. Use Example 💡

Let's make a simple Hello World callback request service.

A simple Hello World Echo Service remote process for this framework might look like the following.

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCStage.drawio.png"></div>

1. Write a hello world back to the service using google's `Protobuf` for parameter messages and result messages

   Note that our `HelloWorldServiceParameters` and `HelloWorldServiceResult` should be in the `DSFramework.DSRPC.Packet` namespace.

   ```protobuf
   syntax = "proto3";
   
   import "google/protobuf/any.proto";
   
   package DSFramework.DSRPC.Packet;
   
   message HelloWorldServiceParameters {
       string name = 1;
       string message = 2;
   }
   
   message HelloWorldServiceResult {
       string message = 1;
   }
   ```

2. Inherits and overrides the `DSFramework::DSRPC::RPCServer::RPCService` class and implements its `ParametersCheck` and `Execute` methods. `ParametersCheck` is used to do a parameter check to see if the parameters are correct and if they are not you will get an `RPCPacketError` with a value of `SERVICE_IVAILD_PARAMETERS` in response to the packet.

   ```cpp
   class HelloWorldService : public RPCServer::RPCService
   {
   public:
   	HelloWorldService() : RPCServer::RPCService("HelloWorldService") {}
   	virtual ~HelloWorldService() = default;
   
   	bool ParametersCheck(std::shared_ptr<RPCPacket> packet) override
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
   
   	void Execute(std::shared_ptr<RPCPacket> packet) override
   	{
   		HelloWorldServiceParameters parameters;
   		packet->parameters().UnpackTo(&parameters);
   		auto name = parameters.name();
   		auto msg = parameters.message();
   		HelloWorldServiceResult result;
   		result.set_message("Hello " + name + " " + msg);
   		google::protobuf::Any* result_any = packet->mutable_result();
   		result_any->PackFrom(result);
   	}
   };
   
   ```

   Of course you can also write `ParametersCheck` and `Execute` directly without inheritance, both of which are `std::function<bool(std::shared_ptr<RPCPacket>)>` and `std::function<void(std::shared_ ptr<RPCPacket>)>`

3. Constructing our Hello World callback request package

   ```c++
   std::shared_ptr<RPCPacket> CreateHelloWorldServiceRequestPacket(std::string name, std::string msg, std::string from, std::string to)
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
   ```

4. Register service into server

   ```
   std::shared_ptr<HelloWorldService> service = std::make_shared<HelloWorldService>();
   TestHelloWorldService testService;
   
   RPCServer server("serverID", "serverName", 9000, 100, 100, 100);
   /// regist example 1 with RPCServer::RPCService class
   server.AddService(service);
   /// regist example 2 without RPCServer::RPCService class
   server.AddService(testService.serviceName, std::bind(&TestHelloWorldService::ParametersCheck, &testService, std::placeholders::_1), std::bind(&TestHelloWorldService::Execute, &testService, std::placeholders::_1));
   /// start it
   server.Start();
   ```

5. Client send packet

   ```cpp
   RPCClient client("clientid", "clientName");
   client.RemoteCall("127.0.0.1", 9000, CreateHelloWorldServiceRequestPacket("Alice", "World", "Client", "Server"), 20);
   ```

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSLog.png"></div>

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCRequestResult.png"></div>

## 5. TO DO 📘

- [x] Remote Procedure Call Creation and Queries
- [x] K-V Engine
- [x] Single Registration Center
- [ ] Distributed Registration Center
- [ ] Route Layer

