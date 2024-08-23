#include <iostream>
#include <memory>
#include <string>

#include "RPCServer.h"


using DSFramework::DSRPC::RPCServer;
int main()
{


	RPCServer server("serverid", "serverName", 9000, 100, 100, 100);
	server.Start();
	return 0;
}
