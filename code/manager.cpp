#include "gtstore.hpp"

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "message.grpc.pb.h"
#include "service/managerService";

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
