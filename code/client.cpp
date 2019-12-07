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
	val_t value = cls.get(key);
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



int main2(int argc, char **argv) {

	if(argc < 2) {
		cout << "no node name. \nex) ./client <some ip or node name>"<<endl;
		exit(EXIT_SUCCESS);
	}

	GTStoreClient client;
	client.init(getpid());

	// string key = argv[1];
	string key = "A";
	vector<string> values;
	values.push_back("a1");
	values.push_back("a2");
	values.push_back("a3");
	client.put(key, values);

	key = "B";
	values.clear();
	values.push_back("b1");
	values.push_back("b2");
	values.push_back("b3");
	client.put(key, values);

	sleep(2);
	key = "A";
	val_t v = client.get(key);
	cout << "key: " << key << endl << "val: ";
	for(auto it=v.begin();it!=v.end();it++) {
		cout << *it << ", ";
	}
	cout <<endl;

	key = "B";
	v = client.get(key);
	cout << "key: " << key << endl << "val: ";
	for(auto it=v.begin();it!=v.end();it++) {
		cout << *it << ", ";
	}
	cout <<endl;

	// key = "C";
	// v = client.get(key);
	// cout << "key: " << v << endl << "val: ";
	// for(auto it=v.begin();it!=v.end();it++) {
	// 	cout << *it << ", ";
	// }
	// cout <<endl;

}
