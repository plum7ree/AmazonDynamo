#include <string>

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

class HashRing{
public:
  std::map<size_t, string> hashMap;
  std::map<string, size_t> pNodeCountMap;

  string get(string k) {
    // loadbalance
    vector<string> pref_list = getPrefList();


  }

  // vector<string> getPrefList(string k, size_t list_size) {
  //   assert(list_size <= hasMap.size());
  //   vector<string> ret;
  //   size_t h = std::hash<string>{}(k);
  //   map<size_t, Node>::iterator it;
  //   it = hashMap.lower_bound(h);
  //   if (it == hasMap.end()) {
  //     it = hashMap.begin();
  //   }
  //   ret.push_back(it->second);
  //   size_t count = 1;
  //   while(count < list_size) {
  //     it++;
  //     if (it == hashMap.end()) {
  //       it = hashMap.begin();
  //     }
  //     // check if already exisintg, because there are multiple vNode for one pNode
  //     if (std::find(ret.begin(), ret.end(), it->second)) {
  //       continue;
  //     }
  //     ret.push_back(it->second);
  //     count++;
  //   }
  //
  //   return ret;
  // }


  void addNode(string node) {
    // assert node name is not exisintg
    if(hasMap.empty()) {
      size_t m = std::numeric_limits<std::size_t>::max()
      size_t partition = std::numeric_limits<std::size_t>::max()/vVNode;
      for(auto i=0; i<nVNode; i++) {
        hashMap.insert({partition, node});
        partition += partition;
      }
    }

  }

  // void removeNode() {
  //
  // }


}




//
// class Node
// pref_list
// ip
//
// addToPrefList
// removeFromPrefList
