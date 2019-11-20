#include "clientService.hpp"

void ClientService::put(string key, val_t value) {
	KeyAndValue kv;
	kv.set_key(key);
	kv.set_value("aa");
	// kv.mutable_value()->CopyFrom(value);
	ClientContext ctx;
	// Status status = stub_->Put(&ctx, KeyAndValue);
}
