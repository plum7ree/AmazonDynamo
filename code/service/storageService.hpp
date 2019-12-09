#include <thread>


#include "../type/helper.hpp"

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

#define PUT 1
#define GET 2


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
//	semaphore sem;
public:
Status Put(ServerContext *ctx, const KeyAndValue *input,  Empty *empty) override;
Status Get(ServerContext *ctx, const Key *input, Value *value) override;
Status Spread(ServerContext *ctx, const KeyAndValue *input,  Empty *empty) override;
int32_t storeValue(string k, val_t v, int32_t vectorClock);
string getIP() {return thisIP;}
void setIP(string ip) {thisIP = ip;}
void printStats();

};
