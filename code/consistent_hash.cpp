#include <map>
#include <string>
#include <vector>
#include <limits>
#include <iostream>

using namespace std;
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
private:
  map<size_t, string> hashMap;
  vector<string> pNodeList;
  size_t nVNode;
  size_t Q_div_S; // nVNode / nPNode

public:
  HashRing(size_t n) : nVNode(n), Q_div_S(n) {
  }
  //
  // string get(string k) {
  //   // loadbalance
  //   vector<string> pref_list = getPrefList();
  //
  //
  // }

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
    if(hashMap.empty()) {
      size_t m = std::numeric_limits<std::size_t>::max();
      size_t partition = m/nVNode;
      for(auto i=0; i<nVNode; i++) {
        hashMap.insert({partition, node});
        partition += partition;
      }
      pNodeList.push_back(node);
      return;
    }

    size_t count = 0;
    Q_div_S = (size_t) nVNode / (pNodeList.size() + 1);
    // while(count < Q_div_S){
    //   auto i = rand() % pNodeList.size();
    //   string replaceThis = pNodeList.at(i);
    //
    //   for(auto it=hashMap.begin(); it != hashMap.end(); it++) {
    //     if(it->second == replaceThis) {
    //         hashMap[it->first] = node;
    //     }
    //   }
    //   count++;
    // }
    size_t howManyReplacePerInstance = Q_div_S / pNodeList.size() +1;
    for(size_t i=0;i<pNodeList.size(); i++) {
      string replaceThis = pNodeList.at(i);
      vector<size_t> replaceThisList;
      for(auto it=hashMap.begin(); it != hashMap.end(); it++) {
        if(it->second == replaceThis) {
          // cout <<"hashmap[it->first]=node" <<endl;
            replaceThisList.push_back(it->first);
        }
      }
      for(size_t j=0;j<howManyReplacePerInstance;j++) {
        auto r = rand() % replaceThisList.size();
        hashMap[replaceThisList[r]] = node;
      }
    }
    pNodeList.push_back(node);



  }

  map<size_t, string> getHashMap() {
    return hashMap;
  }

  // void removeNode() {
  //
  // }


};

int main(int argc, char **argv) {

	HashRing ring(20);
  ring.addNode("a");
  auto hm = ring.getHashMap();
  ring.addNode("b");
  ring.addNode("c");
  ring.addNode("d");
  cout << hm.size() <<endl;
  for(auto it = hm.begin() ; it != hm.end(); it++) {
    cout << "k: " << it->first << ", v: " << it->second <<endl;
  }

  auto hm2 = ring.getHashMap();
  cout << hm2.size() <<endl;
  for(auto it = hm2.begin() ; it != hm2.end(); it++) {
    cout << "k: " << it->first << ", v: " << it->second <<endl;
  }


}



//
// class Node
// pref_list
// ip
//
// addToPrefList
// removeFromPrefList
