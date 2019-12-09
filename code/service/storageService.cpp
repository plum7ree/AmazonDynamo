#include "storageService.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>


StorageServer server;
static map<string,StorageClient> storageConn; // for send k,v to other storage.

// ********************** StorageCleint**********************
// send StorageInfo to Manager
void StorageClient::notifyToManager(string ip) {
  ClientContext ctx;
  StorageInfo input;
  Empty empty;
  input.set_ip(ip);
  cout << "StorageClient::notifyToManager : sending ip to server..." << std::endl;
  Status s =  stub_->notifyToManager(&ctx, input, &empty);
  if (s.ok()) {
      cout << "StorageClient::notifyToManager : notifyToManager rpc succeeded." << std::endl;
    } else {
      cout << "StorageClient::notifyToManager : notifyToManager rpc failed." << std::endl;
    }
}


void StorageClient::spread(string k, val_t value, int32_t vectorClock) {
  ClientContext ctx;
  KeyAndValue kv;
  Empty empty;
  kv.set_key(k);
  _set_msg_value(&kv, value);
  kv.set_vectorclock(vectorClock);
  cout << "StorageClient::spread()" << endl;
  Status s =  stub_->Spread(&ctx, kv, &empty);
  if (s.ok()) {
      cout << "StorageClient::spread : rpc succeeded." << std::endl;
    } else {
      cout << "StorageClient::spread : rpc failed." << std::endl;
    }

}
// ******************************************************************




// ********************** StorageServer ********************************************
// recieve KeyandValue from Manager
Status StorageServer::Put(ServerContext *ctx, const KeyAndValue *input,  Empty *empty) {
  string k = input->key();
  val_t value;
  _get_value_from_msg(value, input);
  int32_t vectorClock = storeValue(k, value, -1);
  cout << "StorageServer::Put value stored! k: " << k << " v[0]: " << value[0] << " size: " << value.size() << " vclock: " << vectorClock << endl;

  PrefListType pl;
  _get_preflist_from_msg(pl, input);

  cout << "StorageServer::Put prefList: ";
  for(auto it=pl.begin();it!=pl.end();it++) {
    cout << *it << " , ";
  }
  cout << endl;

  string TODO = "spread this key value to other storage!!";
  for(auto ip=pl.begin();ip!=pl.end();ip++) {
      if((*ip).compare(getIP()) == 0) continue;

      if(!storageConn.count(*ip)) {
        cout << "StorageServer::Put storage ip: " << *ip << ", ";
        storageConn[*ip] = StorageClient(grpc::CreateChannel(*ip, grpc::InsecureChannelCredentials()));
      }
      storageConn[*ip].spread(k, value, vectorClock);
  }
    usleep(10000); // simulate network latency
    return Status::OK;
}

Status StorageServer::Spread(ServerContext *ctx, const KeyAndValue *input,  Empty *empty) {
  string k = input->key();
  val_t value;
  _get_value_from_msg(value, input);
  int32_t vclock = input->vectorclock();
  storeValue(k,value,vclock);
  cout << "StorageServer::Spread value stored! k: " << k << " v[0]: " << value[0] << " vclock: " << vclock << endl;
  cout << "STATS: size " << inMemoryStorage.size() << endl;


  return Status::OK;
}

Status StorageServer::Get(ServerContext *ctx, const Key *input, Value *value) {
  string k = input->key();
  if(inMemoryStorage.count(k)) {
    final_val_t fv = inMemoryStorage[k];
    value->set_vectorclock(fv.vectorClock);
    _set_msg_value(value, fv.value);
    cout << "StorageServer::Get sending to manager k: " << k << " v[0]: " << fv.value[0] << " size: " << fv.value.size() << " vclock: " << fv.vectorClock << endl;
    return Status::OK;
  }
  return Status(StatusCode::NOT_FOUND, "");
}

int32_t StorageServer::storeValue(string k, val_t v, int32_t vectorClock) {
  int32_t vclock = 0;
  if (vectorClock < 0) { // from PUT
    if (inMemoryStorage.count(k)){ vclock = inMemoryStorage[k].vectorClock + 1;  }
    inMemoryStorage[k] = {vclock, v, 0};
  } else { // from Spread
    inMemoryStorage[k] = {vectorClock, v, 0};
  }

  return vclock;
}

void StorageServer::printStats() {
    ofstream txtOut;
    txtOut.open("/tmp/storage.stats.txt");
    txtOut << inMemoryStorage.size() << endl;
    txtOut.close();
}