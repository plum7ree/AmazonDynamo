#include "clientService.hpp"

void ClientService::put(string& key, val_t& value) {
	cout << "11" << endl;
	KeyAndValue kv;
	kv.set_key(key);
	cout << "value[0]" << value[0] <<endl;
	// kv.set_value(1, value[1]);
	for (auto i=0;i<value.size();i++) {
				kv.add_value(value[i]);
	}

	cout << "11" <<endl;
	// kv.mutable_value()->CopyFrom(value);
	ClientContext ctx;
	Status s =  stub_->Put(&ctx, kv, NULL);

	cout << "11" <<endl;

}
