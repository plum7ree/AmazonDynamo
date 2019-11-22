

#include "gtstore.hpp"
#include "service/storageService.hpp"

// preference list

StorageService ss;

void GTStoreStorage::init() {

	cout << "Inside GTStoreStorage::init()\n";


	ss = StorageService(grpc::CreateChannel(MANAGER_IP, grpc::InsecureChannelCredentials()));

	ss.runStorage("Storage:" + getpid());
	// std::string server_address(ip);

	// ServerBuilder builder;
	// builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// builder.RegisterService(&ss);
	// std::unique_ptr<Server> server(builder.BuildAndStart());
	// std::cout << "Server listening on " << server_address << std::endl;
	// server->Wait();
}



int main(int argc, char **argv) {
	GTStoreStorage storage;
	storage.init();

}
