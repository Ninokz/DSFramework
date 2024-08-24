#include "ConAcceptor.h"
#include "SessionManager.h"
#include "EventHandler.h"
#include "AsyncTcpServer.h"

#include "ConcurrentQueue.h"
#include "ThreadPool.h"
#include "Logger.h"

using namespace DSFramework::DSComponent;
using namespace DSFramework::DSCommunication;

int main()
{
	std::shared_ptr<SessionManager> m_sessionManager = std::make_shared<SessionManager>(100);
	AsyncTcpServer server(9000,100);
	server.AddConnectEventHandler(std::static_pointer_cast<IConnectEventHandler>(m_sessionManager));
	server.AddCloseEventHandler(std::static_pointer_cast<ICloseEventHandler>(m_sessionManager));
	server.Start();
	return 0;
}