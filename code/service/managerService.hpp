
#include <thread>
#include <memory>
#include "../hash/consistent_hash.hpp"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;


using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;

using myMessage::MyMessage;
using myMessage::KeyAndValue;
using myMessage::StorageInfo;
using myMessage::Key;
using myMessage::Value;
using myMessage::Empty;

#define _VNODE_SIZE 			30
#define _PREF_LIST_SIZE 	4
#define PUT 1
#define GET 2


//
// We use strategy 1. route its request through a generic load balancer that will select a
// node based on load information


class ManagerService final: public MyMessage::Service {
private:
	HashRing ring = HashRing((size_t)_VNODE_SIZE, (size_t)_PREF_LIST_SIZE);
public:
	Status Put(ServerContext *ctx, const KeyAndValue *input, Empty *empty) override;
	Status Get(ServerContext *ctx, const Key *input, Value *value) override;

	Status notifyToManager(ServerContext *ctx, const StorageInfo *input,  Empty *empty) override;
	// Status RequestToManager(::grpc::ClientContext* context, const ::myMessage::StorageInfo& request, ::myMessage::ManagerResponse* response);
	// Status SendDataToManager(::grpc::ClientContext* context, const ::myMessage::ValueWithVersion& request, ::google::protobuf::Empty* response);

	void addNode(string node);

};

class ManagerStub {
private:
	std::shared_ptr<MyMessage::Stub> stub_;
public:
	ManagerStub(){}
	ManagerStub(std::shared_ptr<Channel> channel)
	      : stub_(MyMessage::NewStub(channel)) {

	}
	void put(string k, val_t &v, PrefListType &pl);
	final_val_t get(string k);
};


extern map<string, ManagerStub> storageConn;
