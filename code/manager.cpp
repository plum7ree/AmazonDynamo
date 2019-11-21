#include "gtstore.hpp"

#include "service/managerService.hpp"

ManagerService ms;


void GTStoreManager::init() {

	cout << "Inside GTStoreManager::init()\n";

	std::string server_address("127.0.0.1:50051");

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&ms);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

}


int main(int argc, char **argv) {

	GTStoreManager manager;
	manager.init();

}
