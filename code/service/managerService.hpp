
#include <string>
#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "message.grpc.pb.h"

#include "../hash/consistent_hash.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using myMessage::MyMessage;
using myMessage::KeyAndValue;

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
	Status Put(ServerContext *ctx, const KeyAndValue *input, ::google::protobuf::Empty*) override;
	void addNode(string node);

};
