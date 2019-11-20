

#include "../hash/consistent_hash.hpp";

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using myMessage::MyMessage;
using myMessage::Data;


//
// We use strategy 1. route its request through a generic load balancer that will select a
// node based on load information

class ManagerService final: public MyMessage::Service {
private:
	HashRing ring;
public:
	Status Put(ServerContext *ctx, const KeyAndValue *input);

};
