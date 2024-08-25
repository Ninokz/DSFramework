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

- **Request Dispatcher**

  <div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/RPCDispatcher.drawio.png"></div>

  The Dispatcher implements a generic task scheduler that can be used to manage and schedule task requests from different senders. This single concurrent queue can also be used as a logical layer message queue, ensuring that requests are thread-safe.

-  **Response Dispatcher**

  Get response and send it to remote.

### 3.4 Server

Mainly contains a thread pool for executing remote service requests, service name parameters and procedures need to be defined and registered within the component. In this framework, the processor implements this functionality, and the server simply coordinates all of the above modules in one place.

### 3.5 Logger

For logging in multi-threaded environments, you can avoid logging blocking the execution of the main thread.

## 4. Use Example 💡

A simple Hello World Echo Service remote process for this framework might look like the following.

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCStage.drawio.png"></div>

In file [ClientTest.h](https://github.com/Ninokz/DSFramework/blob/master/DSRPC/ClientTest.h)  [HelloWorldTestService.h](https://github.com/Ninokz/DSFramework/blob/master/DSRPC/HelloWorldTestService.h) [ServerTest.h](https://github.com/Ninokz/DSFramework/blob/master/DSRPC/ServerTest.h)

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSRPCServer.png"></div>

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCRequestResult.png"></div>

## 5. TO DO 📘

- [x] Remote Procedure Call Creation and Queries
- [x] K-V Engine
- [x] Single Registration Center
- [ ] Distributed Registration Center
- [ ] Route Layer

