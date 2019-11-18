#include "gtstore.hpp"

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "message.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using myMessage::MyMessage;
using myMessage::Data;

class ManagerService final: public MyMessage::Service {
public:
	Status PingPong(ServerContext *ctx, const Data *input,
		 							Data *output) override{
		cout << "Got Msg from client: " << input->msg() << endl;
		output->set_msg(string("servers response"));
		return Status::OK;
	}
};


void GTStoreManager::init() {

	cout << "Inside GTStoreManager::init()\n";

	std::string server_address("127.0.0.1:50051");
  ManagerService service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();

}


int main(int argc, char **argv) {

	GTStoreManager manager;
	manager.init();

}
