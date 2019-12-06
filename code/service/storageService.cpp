#include "storageService.hpp"

StorageServer server;
map<string,StorageClient> storageConn; // for send k,v to other storage.

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
  for (auto i=0;i<value.size();i++) {
				kv.add_value(value[i]);
	}
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
  for (auto i=0;i<input->value_size();i++) {
				value.push_back(input->value(i));
	}
  int32_t vectorClock = storeValue(k, value, -1);
  cout << "StorageServer::Put value stored! k: " << k << " v[0]: " << value[0] << " vclock: " << vectorClock << endl;


  PrefListType pl;
  for (auto i=0;i<input->preflist_size();i++) {
    pl.push_back(input->preflist(i));
  }

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
  return Status::OK;
}

Status StorageServer::Spread(ServerContext *ctx, const KeyAndValue *input,  Empty *empty) {
  string k = input->key();
  val_t value;
  for (auto i=0;i<input->value_size();i++) {
				value.push_back(input->value(i));
	}
  int32_t vclock = input->vectorclock();
  storeValue(k,value,vclock);
  cout << "StorageServer::Spread value stored! k: " << k << " v[0]: " << value[0] << " vclock: " << vclock << endl;
  string TODO = "TODO: send response to coordinator!";


  return Status::OK;
}

int32_t StorageServer::storeValue(string k, val_t v, int32_t vectorClock) {
  int32_t vclock = 0;
  if (vectorClock < 0) { // from PUT
    if (inMemoryStorage.count(k)){ vclock = inMemoryStorage[k].vectorClock + 1;  }
    inMemoryStorage[k] = {vclock, v};
  } else { // from Spread
    inMemoryStorage[k] = {vectorClock, v};
  }

  return vclock;
}
