
#include <string>
#include <vector>

#include "../type/helper.hpp"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using grpc::StatusCode;

using myMessage::MyMessage;
using myMessage::KeyAndValue;
using myMessage::StorageInfo;
using myMessage::Key;
using myMessage::Value;
using myMessage::Empty;

class ClientService{
public:
	ClientService(){}
	ClientService(std::shared_ptr<Channel> channel)
	      : stub_(MyMessage::NewStub(channel)) {

	}
	void put(string, val_t);
	void put_one_node(string key, val_t value);
	val_t get(string);
private:
	std::unique_ptr<MyMessage::Stub> stub_;
	// std::vector<Feature> feature_list_;
};
