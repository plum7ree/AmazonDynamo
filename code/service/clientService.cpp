#include "clientService.hpp"

void ClientService::put(string key, val_t value) {
	KeyAndValue kv;
	kv.set_key(key);
	kv.set_value(0, value[0]);
	// kv.mutable_value()->CopyFrom(value);
	ClientContext ctx;
	// Status status = stub_->Put(&ctx, KeyAndValue);
}
