#include <iostream>
#include <string>
#include <vector>

#include "ConAcceptor.h"
#include "SessionManager.h"
#include "EventHandler.h"
#include "AsyncTcpServer.h"

#include "ConcurrentQueue.h"
#include "ThreadPool.h"
#include "Logger.h"

using namespace DSFramework::DSComponent;
using namespace DSFramework::DSCommunication;
// Ö÷
int main()
{
	AsyncTcpServer server(9000, 100);
	server.Start();
	return 0;
}