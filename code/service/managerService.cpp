#include "managerService.hpp";

Status Put(ServerContext *ctx, const KeyAndValue *input) override{
  cout << "Got Msg from client: " << input->msg() << endl;
  PrefListType pl = ring.getPrefList(input->key());

  //load balancer
  
  return Status::OK;
}

// Status
