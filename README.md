# AmazonDynamo
Simple Implementation of **Dynamo: amazon's highly available key-value store**
https://www.researchgate.net/publication/220910159_Dynamo_Amazon%27s_highly_available_key-value_store

## Design Consideration 

> 1. Incremental scalability: Dynamo should be able to scale out one
storage host (henceforth, referred to as “node”) at a time, with
minimal impact on both operators of the system and the system
itself.
> 2. Symmetry: Every node in Dynamo should have the same set of
responsibilities as its peers; there should be no distinguished node
or nodes that take special roles or extra set of responsibilities. In
our experience, symmetry simplifies the process of system
provisioning and maintenance.
> 3. Decentralization: An extension of symmetry, the design should
favor decentralized peer-to-peer techniques over centralized
control. In the past, centralized control has resulted in outages and
the goal is to avoid it as much as possible. This leads to a simpler,
more scalable, and more available system.
> 4. Heterogeneity: The system needs to be able to exploit
heterogeneity in the infrastructure it runs on. e.g. the work
distribution must be proportional to the capabilities of the
individual servers. This is essential in adding new nodes with
higher capacity without having to upgrade all hosts at once.

## Consistent Hashing
노드를 추가/제거 하더라도 전체 hash map 을 업데이트 할 필요가 없다. (일부분만 하면됨)
### 1. Virtual Nodes


![image](https://user-images.githubusercontent.com/10215223/113497520-00df1e80-9540-11eb-85d6-ccf9aebaea01.png)
![image](https://user-images.githubusercontent.com/10215223/113497516-f45ac600-953f-11eb-9f7b-a8ab5a5b44d7.png)

1. if a node becomes unavailable, the load handled by this node is evenly dispersed across remaining available nodes
2. if a node becomes available again, new node accpets equivalent amount of node from each of other nodes.
3. number of virtual node depends on capacity, infrastrucutre.

### 2. Replication

1. Coordinator node: node in charge of data items (first node in preference list).
2. Preference list: The list of nodes that is responsible for storing a particular key
    1. failure 에 대비해 N 노드 보다 많이 들어있다. (N 이란 한노드당 replica 개수)
    2. ring 에서 pref list 를 만들때 최소 N개의 다른 physical 노드가 들어있도록 같은 노드가 나오면 skip 한다.
    3. coordinator 는 이 pref list 중 첫번쨰 노드
    4. any request 는 이 pref list 노드중 아무 노드나 선택하는데 만약 top N 노드말고 다른 노드가 선택되면 (failure 대비하기 위한 예비노드) 이 노드로 전파되는게 아니라 coordinator 로 간다.
3. Adding/Removing Storage Nodes (<b>chapter 4.9</b> in paper)
    1. 새로운 Node 가 추가되면, 기존의 노드가 갖고있던 key range 중 일부를 새로운 Node 의 key range 로 바꿔주면됨 (Laod Balancer 의 hashmap 을 수정해주면 됨). (데이터 이동에 대한 말은 논문에는 없음.)

This projects supports
1. Consistent Hashing with hash ring
2. gRPC

