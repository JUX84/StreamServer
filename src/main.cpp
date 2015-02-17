#include "StreamServer.hpp"

#include <Ice/Ice.h>

int main(int argc, char **argv) {
	int status = 0;
	Ice::CommunicatorPtr ic;
	try {
		ic = Ice::initialize(argc, argv);
		Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("StreamServerAdapter", "tcp -p 10000");
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
