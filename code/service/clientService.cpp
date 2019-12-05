#include "clientService.hpp"

void ClientService::put(string& key, val_t& value) {
	KeyAndValue kv;
	kv.set_key(key);
	cout << "value[0]" << value[0] <<endl;
	// kv.set_value(1, value[1]);
	for (auto i=0;i<value.size();i++) {
				kv.add_value(value[i]);
	}

	// kv.mutable_value()->CopyFrom(value);
	ClientContext ctx;
	Empty empty;
	Status s =  stub_->Put(&ctx, kv, &empty);
	if (s.ok()) {
      cout << "StorageClient::notifyToManager : notifyToManager rpc succeeded." << std::endl;
    } else {
      cout << "StorageClient::notifyToManager : notifyToManager rpc failed." << std::endl;
    }

}
