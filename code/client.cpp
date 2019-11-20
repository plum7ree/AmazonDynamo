#include "gtstore.hpp"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "message.grpc.pb.h"

using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

// void ClientService::sendClientId(int id) {
// 	Data cl;
// 	string input = "hello: " + to_string(id);
// 	cl.set_msg(input);
// 	Data svr;
//
// 	ClientContext ctx;
// 	cout << "sending message" << endl;
// 	Status status = stub_->PingPong(&ctx, cl, &svr);
// 	if(!status.ok()) {
// 		cout << "oh man we have problem in pingpong" <<endl;
// 	}
// 	cout<< "Got Msg from server: " << svr.msg() << endl;
// }

void ClientService::put(string key, val_t value) {
	KeyAndValue kv;
	kv.set_key(key);
	kv.set_value(value);
	ClientContext ctx;
	Status status = stub_->Put(&ctx, KeyAndValue);
}

void GTStoreClient::init(int id) {

	cout << "Inside GTStoreClient::init() for client " << id << "\n";
	client_id = id;

	ClientService _cls(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));
	cout <<"client service created!";
	cls = _cls;
	cls.sendClientId(id);

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

	return true;
}

void GTStoreClient::finalize() {

	cout << "Inside GTStoreClient::finalize() for client " << client_id << "\n";
}



int main(int argc, char **argv) {

	GTStoreClient client;
	client.init(getpid());

}
