#include "managerService.hpp"
map<string,ManagerStub> storageConn;

Status ManagerService::Put(ServerContext *ctx, const KeyAndValue *input, ::google::protobuf::Empty*) {
  PrefListType pl = ring.getPrefList(input->key());

  for(auto it=pl.begin(); it != pl.end() ; it++) {
    cout << *it << endl;
  }
  //load balancer
  val_t value;
  for (auto i=0;i<input->value_size();i++) {
				value.push_back(input->value(i));
	}
  ::storageConn[pl[0]].put(input->key(), value);


  return Status::OK;
}

Status ManagerService::notifyToManager(ServerContext *ctx, const StorageInfo *input, ::google::protobuf::Empty*) {
  string storage_ip = input->ip();
  // ManagerStub ms =
  cout <<"storage registered. ip: " << storage_ip << endl;
  ::storageConn[storage_ip] = ManagerStub(grpc::CreateChannel(storage_ip, grpc::InsecureChannelCredentials()));
  addNode(storage_ip);

  return Status::OK;
}

// Status ManagerService::RequestToManager()

void ManagerService::addNode(string node_ip) {
  ring.addNode(node_ip);
}
// Status

void ManagerStub::put(string k, val_t value){
  ClientContext ctx;
  KeyAndValue kv;
  kv.set_key(k);
  for (auto i=0;i<value.size();i++) {
				kv.add_value(value[i]);
	}
	Status s =  stub_->Put(&ctx, kv, NULL);
}
