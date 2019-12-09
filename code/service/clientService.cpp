#include "clientService.hpp"


void ClientService::put(string key, val_t value) {
	KeyAndValue kv;
	kv.set_key(key);
//	cout << "value[0]" << value[0] <<endl;
	// kv.set_value(1, value[1]);
	_set_msg_value(&kv, value);

	// kv.mutable_value()->CopyFrom(value);
	ClientContext ctx;
	Empty empty;
	Status s =  stub_->Put(&ctx, kv, &empty);
	if (s.ok()) {
//    cout << "ClientService::put :  rpc succeeded." << std::endl;
  } else {
//    cout << "ClientService::put :  rpc failed." << std::endl;
  }

}

void ClientService::put_one_node(string key, val_t value) {
	KeyAndValue kv;
	kv.set_key(key);
//	cout << "value[0]" << value[0] <<endl;
	// kv.set_value(1, value[1]);
	_set_msg_value(&kv, value);

	// kv.mutable_value()->CopyFrom(value);
	ClientContext ctx;
	Empty empty;
	Status s =  stub_->PutOneNode(&ctx, kv, &empty);
	if (s.ok()) {
//    cout << "ClientService::put_one_node :  rpc succeeded." << std::endl;
  } else {
//    cout << "ClientService::put_one_node :  rpc failed." << std::endl;
  }

}

val_t ClientService::get(string k) {
	ClientContext ctx;
	Key key;
	Value value;
	key.set_key(k);
	Status s =  stub_->Get(&ctx, key, &value);
	if (s.ok()) {
//    cout << "ClientService::get :  rpc succeeded." << std::endl;
		val_t v;
		_get_value_from_msg(v, &value);
		return v;
  } else {
//    cout << "ClientService::get :  rpc failed." << std::endl;
		return (val_t) 0;
	}
}
