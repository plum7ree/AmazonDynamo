#include "../type/helper.hpp"

//
// class HashRing
// nVNode
// nPNode
// Q_div_S = nVNode / nPNode
//
// get:
// get ip of node among pref list
//
// add:
// each node gives one of their node
//
// remove:
// nodes are distrubted
typedef struct {
  string src_ip;
  string dst_ip;
  size_t start_hash_val;
  size_t partition_size;
} move_content_t;

class HashRing{
private:
  HashMapType hashMap;
  vector<string> pNodeList;
  size_t nVNode;
  size_t Q_div_S; // nVNode / nPNode
  size_t prefListSize;

public:
  HashRing(size_t n, size_t ps) : nVNode(n), Q_div_S(n), prefListSize(ps) {
  }
  //
  // string get(string k) {
  //   // loadbalance
  //   vector<string> pref_list = getPrefList();
  //
  //
  // }

  // void put(string k) {
  //   // auto pref_list = getPrefList();
  // }

  PrefListType getPrefList(string k);
  // void addNode(string node);
  vector<move_content_t> addNode(string node);
  HashMapType getHashMap();

  // void removeNode() {
  //
  // }


};




//
// class Node
// pref_list
// ip
//
// addToPrefList
// removeFromPrefList
