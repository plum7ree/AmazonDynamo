#include "gtstore.hpp"
#include "service/clientService.hpp"


ClientService cls;


void GTStoreClient::init(int id) {

	cout << "Inside GTStoreClient::init() for client " << id << "\n";
	client_id = id;

	cls = ClientService(grpc::CreateChannel(MANAGER_IP, grpc::InsecureChannelCredentials()));
	cout <<"client service created!"<<endl;

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

	if(argc < 2) {
		cout << "no node name. \nex) ./client <some ip or node name>"<<endl;
		exit(EXIT_SUCCESS);
	}

	GTStoreClient client;
	client.init(getpid());

	vector<string> values;
	values.push_back("data1");
	values.push_back("data2");
	values.push_back("data3");
	client.put(argv[1], values);

}
