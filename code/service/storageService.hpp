#include <string>
#include <vector>


#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "message.grpc.pb.h"



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
using myMessage::ManagerResponse;
using myMessage::ValueWithVersion;

#define PUT 1
#define GET 2

typedef vector<string> val_t;

//
// #include <grpc/grpc.h>
// #include <grpcpp/server.h>
// #include <grpcpp/server_builder.h>
// #include <grpcpp/server_context.h>
// #include <grpcpp/security/server_credentials.h>
// #include "message.grpc.pb.h"

// using grpc::Server;
// using grpc::ServerBuilder;
// using grpc::ServerContext;
// using grpc::ServerReader;
// using grpc::ServerReaderWriter;
// using grpc::ServerWriter;
// using grpc::Status;
// using myMessage::MyMessage;
// using myMessage::KeyAndValue;


//
// class StorageService : public MyMessage::Service  {
// private:
//   unordered_map<string, val_t> inMemoryStorage;
//
// public:
//   Status PutToStorage(ServerContext *ctx, const ManagerToStorage *input, ::google::protobuf::Empty*) override;
//   Status GetFromStorage(ServerContext *ctx, const Key *input, ValueWithVersion *output) override;
// }
class StorageService{
public:
	StorageService(){}
	StorageService(std::shared_ptr<Channel> channel)
	      : stub_(MyMessage::NewStub(channel)) {

	}
  void registerToManager(string ip);
  void runStorage(string ip);
  void storeDataFromManager(string k, val_t v);
  void sendDataToManager(ClientContext& ctx, string k);
private:
	std::unique_ptr<MyMessage::Stub> stub_;
  unordered_map<string, val_t> inMemoryStorage;
	// std::vector<Feature> feature_list_;
};
