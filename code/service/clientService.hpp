
#include <string>
#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "message.grpc.pb.h"

using namespace std;

using myMessage::MyMessage;
using myMessage::KeyAndValue;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

typedef vector<string> val_t;

class ClientService{
public:
	ClientService(){}
	ClientService(std::shared_ptr<Channel> channel)
	      : stub_(MyMessage::NewStub(channel)) {

	}
	void put(string& key, val_t& value);
private:
	std::unique_ptr<MyMessage::Stub> stub_;
	// std::vector<Feature> feature_list_;
};
