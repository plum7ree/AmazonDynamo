

#include "gtstore.hpp"
#include "service/storageService.hpp"


pthread_cond_t cond;
pthread_mutex_t _lock;

StorageClient storageClient;
StorageServer storageServer;

void notifyToManager(string storage_address) {
	pthread_cond_wait(&cond, &_lock);
	cout <<"initializing stub..."<<endl;
	storageClient = StorageClient(grpc::CreateChannel(MANAGER_IP, grpc::InsecureChannelCredentials()));
	storageClient.notifyToManager(storage_address);
	cout <<"notified to Mananger"<<endl;
}



void GTStoreStorage::init() {

	cout << "Inside GTStoreStorage::init()\n";
	pthread_mutex_init(&_lock, NULL);

	// std::string server_address((string)MANAGER_IP);
	pthread_mutex_lock(&_lock);
	std::string server_address = IP_BASE + to_string(getpid() + PORT_BASE);
	cout << "address: " <<server_address<< endl;

	thread _notifyToManager(notifyToManager, server_address);

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&storageServer);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Storage Server listening on " << server_address << std::endl;
	storageServer.setIP(server_address);
	pthread_mutex_unlock(&_lock);
	pthread_cond_signal(&cond);
	// ::_ready = true;
	server->Wait();





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
