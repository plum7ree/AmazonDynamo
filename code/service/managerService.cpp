#include "managerService.hpp"

Status ManagerService::Put(ServerContext *ctx, const KeyAndValue *input, ::google::protobuf::Empty*) {
  PrefListType pl = ring.getPrefList(input->key());

  for(auto it=pl.begin(); it != pl.end() ; it++) {
    cout << *it << endl;
  }
  //load balancer


  return Status::OK;
}

void ManagerService::addNode(string node) {
  ring.addNode(node);
}
// Status
