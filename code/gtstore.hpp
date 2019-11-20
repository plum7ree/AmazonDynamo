#ifndef GTSTORE
#define GTSTORE

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

using grpc::Channel;
using myMessage::MyMessage;
using myMessage::Data;

typedef vector<string> val_t;

class GTStoreClient {
private:
	int client_id;
	val_t value;
	ClientService cls;
public:
	void init(int id);
	void finalize();
	val_t get(string key);
	bool put(string key, val_t value);
};

class GTStoreManager {
public:
	void init();
};

class GTStoreStorage {
public:
	void init();
};


class ClientService{
public:
	ClientService(std::shared_ptr<Channel> channel)
	      : stub_(MyMessage::NewStub(channel)) {

	}
	void sendClientId(int id);
	

private:

	std::unique_ptr<MyMessage::Stub> stub_;
	// std::vector<Feature> feature_list_;
};


#endif
