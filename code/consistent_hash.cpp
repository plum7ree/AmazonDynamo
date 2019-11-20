#include <map>
#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>


#include <time.h>
#include <stdlib.h>
#include <assert.h>


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

  void put(string k) {
    // auto pref_list = getPrefList();
  }

  vector<string> getPrefList(string k) {
    assert(prefListSize <= hashMap.size());
    vector<string> ret;
    size_t h = std::hash<string>{}(k);
    auto it = hashMap.lower_bound(h);
    if (it == hashMap.end()) {
      it = hashMap.begin();
    }
    ret.push_back(it->second);
    size_t count = 1;
    while(count < prefListSize) {
      it++;
      if (it == hashMap.end()) {
        it = hashMap.begin();
      }
      // check if already exisintg, because there are multiple vNode for one pNode
      auto it2 = find(ret.begin(), ret.end(), it->second);
      if(it2 != ret.end()) {
        continue;
      }
      ret.push_back(it->second);
      count++;
    }


    cout << "hash: "<< h << endl << "List: ";
    for(auto it=ret.begin();it!=ret.end();it++) {
      cout << *it <<", ";
    }
    cout << endl;

    return ret;
  }


  void addNode(string node) {
    // assert node name is not exisintg
    if(hashMap.empty()) {
      size_t m = std::numeric_limits<std::size_t>::max();
      cout << "max: " << m << endl;
      size_t partition = m/nVNode;
      size_t location = 0;
      for(auto i=0; i<nVNode; i++) {
        hashMap.insert({location, node});
        location += partition;
      }
      pNodeList.push_back(node);
      return;
    }

    // size_t count = 0;
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
    srand(time(NULL));
    // size_t howManyReplacePerInstance = Q_div_S / pNodeList.size();
    // if(howManyReplacePerInstance == 0) {howManyReplacePerInstance = 1;}
    for(size_t i=0;i<pNodeList.size(); i++) {
      string replaceThis = pNodeList.at(i);
      vector<size_t> replaceThisList;
      for(auto it=hashMap.begin(); it != hashMap.end(); it++) {
        if(it->second == replaceThis) {
          // cout <<"hashmap[it->first]=node" <<endl;
            replaceThisList.push_back(it->first);
        }
      }
      int howManyReplacePerInstance = replaceThisList.size() - Q_div_S;  // don't replace only one left.
      // cout << "node:" << replaceThis <<", hMRPI: " << howManyReplacePerInstance <<endl;
      vector<size_t> alreadyReplaced;
      for(int j=0;j<howManyReplacePerInstance;j++) {
        auto r = rand() % replaceThisList.size();
        auto it = find(alreadyReplaced.begin(), alreadyReplaced.end(),r);
        if(it != alreadyReplaced.end()) {j--; continue;}
        hashMap[replaceThisList[r]] = node;
        alreadyReplaced.push_back(r);
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

	HashRing ring(20,3);
  ring.addNode("a");
  auto hm = ring.getHashMap();
  ring.addNode("b");
  ring.addNode("c");
  ring.addNode("d");
  ring.addNode("e");
  // ring.addNode("f");
  cout << hm.size() <<endl;
  for(auto it = hm.begin() ; it != hm.end(); it++) {
    cout << "k: " << it->first << ", v: " << it->second <<endl;
  }

  auto hm2 = ring.getHashMap();
  cout << hm2.size() <<endl;
  for(auto it = hm2.begin() ; it != hm2.end(); it++) {
    cout << "k: " << it->first << ", v: " << it->second <<endl;
  }

  auto l = ring.getPrefList("node1:50051");

}



//
// class Node
// pref_list
// ip
//
// addToPrefList
// removeFromPrefList
