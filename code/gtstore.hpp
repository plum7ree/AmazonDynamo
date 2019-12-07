#ifndef GTSTORE
#define GTSTORE

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>


using namespace std;
#define MANAGER_IP "127.0.0.1:50051"
#define IP_BASE "127.0.0.1:"
#define PORT_BASE 50000

typedef vector<string> val_t;


class GTStoreClient {
private:
	int client_id;
	val_t value;
	bool ready_;
public:
	void init(int id);
	void finalize();
	val_t get(string key);
	bool put(string key, val_t value);
	bool put_only_to_one_node(string key, val_t value);
};

class GTStoreManager {
public:
	void init();
};

class GTStoreStorage {
private:
	// unordered_map<string, string> inMemoryStorage;
public:
	void init();
};



#endif
