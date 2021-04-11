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

## 1. Partitioning

![image](https://user-images.githubusercontent.com/10215223/113497516-f45ac600-953f-11eb-9f7b-a8ab5a5b44d7.png)

기존의 retational DB(SQL)는 strong consistency 때문에 partition 이 불가능함 (partition 을 하면 strong consistency 가 어려움). 따라서 dynamo는 nosql 기반 (key-value 기반.)

> Traditional replicated relational database systems focus on the problem of guaranteeing strong consistency to replicated data. Although strong consistency provides the application writer a convenient programming model, these systems are limited in scalability and availability [7]. These systems are not capable of handling network partitions because they typically provide strong consistency guarantees.

### 기존 consistency hashing 의 문제점

만일 키 요청이 특정 area 에만 몰리면, 그 키값 범위의 노드만 데이터가 많아짐. (non-uniform, not equal load distribution)
노드간의 performance 가 다를 수 있음을 고려하지안음.

> The basic consistent hashing algorithm presents some challenges. First, the random position assignment of each node on the ring leads to non-uniform data and load distribution. Second, the basic algorithm is oblivious to the heterogeneity in the performance of nodes.

### Dynamo의 Virtual Node 로 해결
single point 에 저장하는대신 한 노드가 여러 partition 을 담당함.
#### 이점.
한 노드 장애 발생시 → 다른 이용가능한 노드가 data 를 핸들
새 노드 추가시 → 다른 노드만큼의 equivalent amount of load 를 가짐
한 노드가 담당하는 virtual node 의 갯수는 그 노드의 성능에 따라 다르게 설정할수있음 (인프라의 heterogeneity 대응 가능)

## 2. Replication

1. Coordinator node: node in charge of data items (first node in preference list).
2. Preference list: The list of nodes that is responsible for storing a particular key
    1. failure 에 대비해 N 노드 보다 많이 들어있다. (N 이란 한노드당 replica 개수)
    2. ring 에서 pref list 를 만들때 최소 N개의 다른 physical 노드가 들어있도록 같은 노드가 나오면 skip 한다.
    3. coordinator 는 이 pref list 중 첫번쨰 노드
    4. any request 는 이 pref list 노드중 아무 노드나 선택하는데 만약 top N 노드말고 다른 노드가 선택되면 (failure 대비하기 위한 예비노드) 이 노드로 전파되는게 아니라 coordinator 로 간다.
3. Adding/Removing Storage Nodes (<b>chapter 4.9</b> in paper)
    1. 새로운 Node 가 추가되면, 기존의 노드가 갖고있던 key range 중 일부를 새로운 Node 의 key range 로 바꿔주면됨 (Laod Balancer 의 hashmap 을 수정해주면 됨). (데이터 이동에 대한 말은 논문에는 없음.)



This projects supports
1. Partitioning with Consistent Hash.
2. gRPC

