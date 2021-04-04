# AmazonDynamo
Simple Implementation of **Dynamo: amazon's highly available key-value store**
https://www.researchgate.net/publication/220910159_Dynamo_Amazon%27s_highly_available_key-value_store

##Consistent Hashing
1. Virtual Nodes


![image](https://user-images.githubusercontent.com/10215223/113497520-00df1e80-9540-11eb-85d6-ccf9aebaea01.png)
![image](https://user-images.githubusercontent.com/10215223/113497516-f45ac600-953f-11eb-9f7b-a8ab5a5b44d7.png)

if a node becomes unavailable, the load handled by this node is evenly dispersed across remaining available nodes
if a node becomes available again, new node accpets equivalent amount of node from each of other nodes.
number of virtual node depends on capacity, infrastrucutre.

2. Replication

Coordinator node: node in charge of data items.
Preference list: The list of nodes that is responsible for storing a particular key
    1. failure 에 대비해 N 노드 보다 많이 들어있다. (N 이란 한노드당 replica 개수)
    2. ring 에서 pref list 를 만들때 최소 N개의 다른 phsyical 노드가 들어있도록 같은 노드가 나오면 skip 한다.
    3. coordinator 는 이 pref list 중 첫번쨰 노드
    4. any request 는 이 pref list 노드중 아무 노드나 선택하는데 만약 top N 노드말고 다른 노드가 선택되면 (failure 대비하기 위한 예비노드) 이 노드로 전파되는게 아니라 coordinator 로 간다.


This projects supports
1. Consistent Hashing with hash ring
2. gRPC

