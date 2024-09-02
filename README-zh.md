# 🌠DSFramework

## 1. Description 📖

基于 `Boost Asio` 和 `Protobuf` 库的 C++ 11 简单远程过程调用框架，其核心是主从 Reactor 多线程模型和 `Boost Asio` 异步读写机制。

## 2. Dependency ⚙

- [Boost 1.8.5](https://www.boost.org/users/history/version_1_85_0.html)
- [Google protobuf 4.25.1](https://github.com/protocolbuffers/protobuf/tree/main/src)

## 3. Design 📏

框架的基本结构如下

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSC.drawio.png"></div>

目前该框架支持简单点对点远程过程调用，发现或管理服务实例的集中式服务正在测试中。下面简要介绍几个功能层。

### 3.1 Connection  Layer

- **Asio-Connection acceptor**

  使用 `async_accept` 异步处理连接请求、构建会话并将其注册到 Asio IO 服务池
  

### 3.2 Communication Layer

- **Asio IO-Service Pool**

  <div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCIOServicepool.drawio.png"></div>

  `IOServicePool` 是一个管理多个 `io_context` 对象的池。每个 `io_context` 都由单独的线程处理，以分配 I/O 操作

- **Session Manager**

  简单的会话管理器用于在连接建立后管理会话，重点是连接和关闭事件

- **Communication Event Handler**

  定义了一种事件处理机制，用于处理与网络通信相关的各种事件，如连接、关闭连接和**数据接收**

### 3.3 Stub

- **Serializer / Deserializer**

  基于 `Protobuf` 的序列化和反序列化数据数据

- **RPCEventHandler**

  <div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSRPCEventhandler.drawio.png"></div>

  框架应用层的事件调度器，属于核心组件部分，本层各组件的交互通过此组件解耦，同时依赖于此组件

- **Request Dispatcher**

  <div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/RPCDispatcher.drawio.png"></div>

  Dispatcher 实现了一个通用任务调度器，可用于管理和调度来自不同发送方的任务请求。这个单一并发队列还可用作逻辑层消息队列，确保请求是线程安全的.

-  **Response Dispatcher**

  获取响应并发送至远程节点

### 3.4 Server

主要包含一个用于执行远程服务请求的线程池，服务名称参数和程序需要在组件内定义和注册。在这个框架中，处理器实现了这些功能，而服务器只需在一个地方协调上述所有模块。

### 3.5 Logger

对于多线程环境中的日志记录，可以避免日志记录阻塞主线程的执行

## 4. Use Example 💡

该框架的一个简单 Hello World Echo 服务远程进程可能如下所示

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCStage.drawio.png"></div>

In file [ClientTest.h](https://github.com/Ninokz/DSFramework/blob/master/DSRPC/ClientTest.h)  [HelloWorldTestService.h](https://github.com/Ninokz/DSFramework/blob/master/DSRPC/HelloWorldTestService.h) [ServerTest.h](https://github.com/Ninokz/DSFramework/blob/master/DSRPC/ServerTest.h)

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSRPCServer.png"></div>

<div align=center><img src="https://observer-storage.oss-cn-chengdu.aliyuncs.com/github/DSFramework/DSCRequestResult.png"></div>

## 5. TO DO 📘

- [x] 远程过程调用的创建和查询
- [x] K-V引擎
- [x] 单一注册中心
- [ ] 分布式注册中心
- [ ] 路由层

