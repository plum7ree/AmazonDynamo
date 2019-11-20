#include "gtstore.hpp"
#include "service/clientService.hpp"


ClientService cls;


void GTStoreClient::init(int id) {

	cout << "Inside GTStoreClient::init() for client " << id << "\n";
	client_id = id;

	cls = ClientService(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));
	cout <<"client service created!";

}

val_t GTStoreClient::get(string key) {

	cout << "Inside GTStoreClient::get() for client: " << client_id << " key: " << key << "\n";
	val_t value;
	// Get the value!
	return value;
}

bool GTStoreClient::put(string key, val_t value) {

	string print_value = "";
	for (uint i = 0; i < value.size(); i++) {
		print_value += value[i] + " ";
	}
	cout << "Inside GTStoreClient::put() for client: " << client_id << " key: " << key << " value: " << print_value << "\n";
	// Put the value!
	cls.put(key, value);

	return true;
}

void GTStoreClient::finalize() {

	cout << "Inside GTStoreClient::finalize() for client " << client_id << "\n";
}



int main(int argc, char **argv) {

	GTStoreClient client;
	client.init(getpid());

}
