#include "gtstore.hpp"

#include "service/managerService.hpp"

unordered_map<string,shared_ptr<Channel>> storage_map;
ManagerService ms;



void GTStoreManager::init() {

	cout << "Inside GTStoreManager::init()\n";
	std::string server_address((string)MANAGER_IP);

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&ms);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();


}

// void GTStoreManager::addNode(string node_ip) {
// 	shared_ptr<Channel> channel = grpc::CreateChannel(node_ip, grpc::InsecureChannelCredentials());
// 	storage_map[node_ip] = MyMessage::NewStub(channel);
// 	ms.addNode(node);
// }


int main(int argc, char **argv) {
	// if(argc != 2) {
	// 	cout << "please provide manager ip";
	// 	exit();
	// }

	GTStoreManager manager;
	manager.init();

}
