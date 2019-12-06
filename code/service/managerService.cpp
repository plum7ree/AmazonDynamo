#include "managerService.hpp"
map<string,ManagerStub> storageConn;

// ***************** managerService class  *****************

// server receive this from client
Status ManagerService::Put(ServerContext *ctx, const KeyAndValue *input, Empty *empty) {
  string WARNING = " NEVER CALL input->key() MORE THAN ONCE ";
  string k = input->key();
  cout <<"key value from client: " + k << endl;
  PrefListType pl = ring.getPrefList(k);
  // PrefListType pl = {"node1" , "node2"};
  cout << "preference list of size " << pl.size() << ": ";
  for(auto it=pl.begin(); it != pl.end() ; it++) {
    cout << *it << ", ";
  }
  cout << endl;

  val_t value;
  for (auto i=0;i<input->value_size();i++) {
				value.push_back(input->value(i));
	}
  int load_balance_res = rand() % pl.size();
  // server sends to storage
  ::storageConn[pl[load_balance_res]].put(k, value, pl);


  return Status::OK;
}

Status ManagerService::notifyToManager(ServerContext *ctx, const StorageInfo *input, Empty *empty) {
  string storage_ip = input->ip();
  // ManagerStub ms =
  ::storageConn[storage_ip] = ManagerStub(grpc::CreateChannel(storage_ip, grpc::InsecureChannelCredentials()));
  addNode(storage_ip);
  cout <<"!ManagerService::notifyToManager: storage registered to hash ring. ip: " << storage_ip << endl;


  return Status::OK;
}

// Status ManagerService::RequestToManager()

void ManagerService::addNode(string node_ip) {
  ring.addNode(node_ip);
}
// Status



// ***************** ManagerStub class  *****************

void ManagerStub::put(string k, val_t &value, PrefListType &pl){
  ClientContext ctx;
  KeyAndValue kv;
  Empty empty;
  kv.set_key(k);
  for (auto i=0;i<value.size();i++) {
				kv.add_value(value[i]);
	}
  for (auto it=pl.begin();it!=pl.end();it++) {
    kv.add_preflist(*it);
  }
	Status s =  stub_->Put(&ctx, kv, &empty);
}
