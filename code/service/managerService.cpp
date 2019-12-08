#include "managerService.hpp"



static map<string,ManagerStub> storageConn;

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

// for test version control. preflist size is always one(coordinator).
Status ManagerService::PutOneNode(ServerContext *ctx, const KeyAndValue *input, Empty *empty) {
  string WARNING = " NEVER CALL input->key() MORE THAN ONCE ";
  string k = input->key();
  cout <<"key value from client: " + k << endl;
  PrefListType pl = {ring.getPrefList(k)[0]};
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

void ManagerService::addNode(string new_node_ip) {
  vector<move_content_t> moveContent = ring.addNode(new_node_ip);
  // cout<<"ManagerService::addNode move content size: " << moveContent.size() << endl;
  // for(auto it=moveContent.begin();it!=moveContent.end();it++) {
  //   cout << "hash: "<< it->start_hash_val << " original node: " << it->src_ip << endl;
  //   // string src_ip;
  //   // string dst_ip;
  //   // size_t start_hash_val;
  //   // size_t partition_size;
  //   ::storageConn[it->src_ip].moveContent(it->start_hash_val, it->partition_size, new_node_ip);
  //
  //
  // }
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

// void ManagerStub::moveContent(size_t start_hash_val, size_t partition_size, string dst_ip){
//   ClientContext ctx;
//   HashRange hrange;
//   KeyAndValue kv;
//   hrange.set_starthash(start_hash_val);
//   hrange.set_partitionsize(partition_size);
//   unique_ptr<ClientReader<KeyAndValue>> reader(
//       stub_->PopContent(&ctx, hrange));
//   // ::storageConn[node_ip].pushContent()
//   // Status s = MoveContent()
//   while (reader->Read(&kv)) {
//     string k = kv.key();
//     val_t value;
//     _get_value_from_msg(value, &kv);
//     cout << "key recieved! k: " << k << endl;
//     ClientContext ctx;
//     KeyAndValue kv2;
//     Empty empty;
//     PrefListType pl = {dst_ip};
//     kv2.set_key(k);
//     _set_msg_value(&kv2, value);
//     _set_msg_preflist(&kv2, pl);
//     ::storageConn[dst_ip].getStub()->Put(&ctx, kv, &empty);
//   }
//   Status status = reader->Finish();
//   if (status.ok()) {
//     std::cout << "ManagerService::moveContent rpc succeeded." << std::endl;
//   } else {
//     std::cout << "ManagerService::moveContent rpc failed." << std::endl;
//   }
// }
