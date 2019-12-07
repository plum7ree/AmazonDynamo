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
  _get_value_from_msg(value, input);
  int load_balance_res = rand() % pl.size();
  // server sends to storage
  ::storageConn[pl[load_balance_res]].put(k, value, pl);


  return Status::OK;
}

Status ManagerService::Get(ServerContext *ctx, const Key *input, Value *value) {
  string k = input->key();
  PrefListType pl = ring.getPrefList(k);
  final_val_t fv;
  final_val_t recent_val;
  recent_val.vectorClock = -1;
  vector<string> needToUpdate;

  for (auto it=pl.begin(); it != pl.end(); it++) {
    fv = ::storageConn[*it].get(k);
    if(!fv.empty && (fv.vectorClock > recent_val.vectorClock)) {
      recent_val = fv;
    } else {
      needToUpdate.push_back(*it);
    }
  }

 // TODO:
 string TODO = "TODO: create another thread and update storages with 'needToUpdate'";

 //


  if(recent_val.vectorClock >= 0) {
    _set_msg_value(value, recent_val.value);

    return Status::OK;
  }

  return Status(StatusCode::NOT_FOUND, "");

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
  _set_msg_value(&kv, value);
  _set_msg_preflist(&kv, pl);
	Status s =  stub_->Put(&ctx, kv, &empty);
}

final_val_t ManagerStub::get(string k) {
  ClientContext ctx;
  Key key;
  Value value;
  key.set_key(k);
  Status s = stub_->Get(&ctx, key, &value);
  final_val_t fv;
  fv.empty = 1;
  if (s.ok()) {
    fv.vectorClock = value.vectorclock();
    _get_value_from_msg(fv.value, (const Value*)&value);
    fv.empty = 0;
  }
  return fv;
}
