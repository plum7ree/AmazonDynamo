#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "message.grpc.pb.h"

#include "../type/type.hpp"

// *************************************************************
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
//***************************************************************

using namespace std;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using myMessage::MyMessage;
using myMessage::KeyAndValue;
using myMessage::StorageInfo;
using myMessage::Empty;

#define PUT 1
#define GET 2

typedef struct {
	int32_t vectorClock;
	val_t v;
} final_val_t;



class StorageClient { // used for initial connection with server or sending data to other storages...
public:
	StorageClient(){}
	StorageClient(std::shared_ptr<Channel> channel)
	      : stub_(MyMessage::NewStub(channel)) {

	}
	void notifyToManager(string ip);
	void spread(string k, val_t value, int32_t vectorClock);

private:
	std::shared_ptr<MyMessage::Stub> stub_;
};



class StorageServer : public MyMessage::Service {
private:
	string thisIP;
	unordered_map<string, final_val_t> inMemoryStorage;
public:
Status Put(ServerContext *ctx, const KeyAndValue *input,  Empty *empty);
Status Spread(ServerContext *ctx, const KeyAndValue *input,  Empty *empty);
int32_t storeValue(string k, val_t v, int32_t vectorClock);
string getIP() {return thisIP;}
void setIP(string ip) {thisIP = ip;}

};
