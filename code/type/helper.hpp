#include "type.hpp"
#include "grpc_include.hpp"

using myMessage::KeyAndValue;
using myMessage::Key;
using myMessage::Value;

void _get_value_from_msg(val_t &dest, const Value *input);
void _get_value_from_msg(val_t &dest, const KeyAndValue *input);
void _get_preflist_from_msg(PrefListType &pl, const KeyAndValue *input);

void _set_msg_value(Value *value, val_t src);
void _set_msg_value(KeyAndValue *value, val_t src);
void _set_msg_preflist(KeyAndValue *kv, PrefListType src);
