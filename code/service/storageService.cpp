#include "storageService.hpp"

// Status StorageService::PutToStorage(ServerContext *ctx, const ManagerToStorage *input, ::google::protobuf::Empty*) {
//   string k = input->key();
//   vector<string> v;
//   size_t size = input->value_size();
//   for(size_t i =0; i < size; i++) {
//     v.push_back(input->value(i));
//   }
//   inMemoryStorage[k] = v;
//
//   // for(auto it=pl.begin(); it != pl.end() ; it++) {
//   //   cout << *it << endl;
//   // }
//   // //load balancer
//
//
//   return Status::OK;
// }
//
// Status StorageService::GetFromStorage(ServerContext *ctx, const Key *input, ValueWithVersion *output) {
//   auto it = inMemoryStorage.find(input->key());
//   if(it == inMemoryStorage.end()) {
//     //
//   } else {
//     auto value = *it;
//     for (auto i=0;i<value.size();i++) {
//   				output.add_value(value[i]);
//   	}
//   }
//   return Status::OK;
// }

void StorageService::registerToManager(string ip) {
  ClientContext ctx;
  StorageInfo input;
  ManagerResponse response;
  input.set_ip(ip);
  Status s = stub_->RequestToManager(ctx, ip, response);
}
void StorageService::runStorage(string ip){
  while(true) {
    ClientContext ctx;
    StorageInfo input;
    ManagerResponse response;
    input.set_ip(ip);
    Status s =  stub_->RequestToManager(&ctx, input, &response);
    string k = response.key();

    if (response.type() == PUT) {
      val_t v;
      vector<string> otherNodes;
      for (auto i=0;i<response.value_size();i++) {
    				v.push_back(response.value(i));
    	}
      for (auto i=0;i<response.othernodes_size();i++) {
    				otherNodes.push_back(response.othernodes(i));
    	}
      storeDataFromManager(k,v);
      //spread to other nodes
    } else if (response.type() == GET) {
      sendDataToManager(ctx, k);
    }
  }


}

void StorageService::storeDataFromManager(string k, val_t v){
  inMemoryStorage[k] = v;
}

void StorageService::sendDataToManager(ClientContext& ctx, string k){
  ValueWithVersion vwv;
  vwv.set_clock(string("time"));
  for (auto i=0;i<inMemoryStorage[k].size();i++) {
        vwv.add_value(inMemoryStorage[k][i]);
  }

  Status s = stub_->SendDataToManager(&ctx, vwv, NULL);
}
