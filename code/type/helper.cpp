#include "helper.hpp"


void _get_value_from_msg(val_t &dest, const Value *input) {
  for (auto i=0;i<input->value_size();i++) {
        dest.push_back(input->value(i));
  }
}

void _get_value_from_msg(val_t &dest, const KeyAndValue *input) {
  for (auto i=0;i<input->value_size();i++) {
        dest.push_back(input->value(i));
  }
}

void _get_preflist_from_msg(PrefListType &pl, const KeyAndValue *input) {
  for (auto i=0;i<input->preflist_size();i++) {
    pl.push_back(input->preflist(i));
  }
}



void _set_msg_value(Value *value, val_t src) {
  for(auto it=src.begin();it!=src.end();it++) {
    value->add_value(*it);
  }
}

void _set_msg_value(KeyAndValue *value, val_t src) {
  for(auto it=src.begin();it!=src.end();it++) {
    value->add_value(*it);
  }
}

void _set_msg_preflist(KeyAndValue *kv, PrefListType src) {
  for (auto it=src.begin();it!=src.end();it++) {
    kv->add_preflist(*it);
  }
}
// void _set_value_to_msg(val_t value, Value &value_to_msg) {
//
// }
