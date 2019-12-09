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

//#define CONCURRENCY 100
//
//class semaphore
//{
//private:
//    std::mutex mutex_;
//    std::condition_variable condition_;
//    unsigned long count_ = CONCURRENCY; // Initialized as locked.
//
//public:
//    void notify() {
//        std::lock_guard<decltype(mutex_)> lock(mutex_);
//        ++count_;
//        condition_.notify_one();
//    }
//
//    void wait() {
//        std::unique_lock<decltype(mutex_)> lock(mutex_);
//        while(!count_) // Handle spurious wake-ups.
//            condition_.wait(lock);
//        --count_;
//    }
//
//    bool try_wait() {
//        std::lock_guard<decltype(mutex_)> lock(mutex_);
//        if(count_) {
//            --count_;
//            return true;
//        }
//        return false;
//    }
//};
