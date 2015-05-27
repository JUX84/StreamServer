#include "StreamServer.hpp"

#include <cstdlib>
#include <Ice/Ice.h>

int main(int argc, char **argv) {
	long port;
	if(argc != 2) {
		std::cerr << "Specify port in argument\n";
		return 1;
	} else {
		try {
			std::string tmp = argv[1];
			port = std::stoul(tmp);
		} catch (const std::exception& e) {
			std::cerr << "Invalid port number\n";
			return 1;
		}
	}
	system("exec rm songs/*");
	int status = 0;
	Ice::CommunicatorPtr ic;
	try {
		ic = Ice::initialize();
		std::string opts = "tcp -p " + std::to_string(port);
		Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("StreamServerAdapter", opts.c_str());
		Ice::ObjectPtr object = new StreamServer;
		adapter->add(object, ic->stringToIdentity("StreamServer"));
		adapter->activate();
		ic->waitForShutdown();
	} catch (const Ice::Exception& e) {
		std::cerr << e << '\n';
		status = 1;
	} catch (const char* msg) {
		std::cerr << msg << '\n';
		status = 1;
	}
	if (ic) {
		try {
			ic->destroy();
		} catch (const Ice::Exception& e) {
			std::cerr << e << '\n';
			status = 1;
		}
	}
	return status;
}
