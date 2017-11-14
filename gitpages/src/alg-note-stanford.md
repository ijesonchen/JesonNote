---
title: Note for Stan ford Algorithm
date: 2017-05-24 22:14:00
tags: [alg, note, shortest path]
---

#  Stanford Algorithm Note

Stanford algorithm note.

<!--more-->



# Graph Primitives (w5)

## BFS: Breadth First Search

 - explorer in layer
 - app: shortest path
 - app: connected component for undirected graph



## Undirected Connectivity 



## DFS: Depth First Search 

 - explorer aggressively
 - app: topological ordering of DAG
 - app: strong connected component



## Topological Sort



## Strongly Connected Components: Kosaraju's Alogrithm

### 1. Kosaraju Alg: two pass dfs (clrs 22.5)

```
计算G及G的转置（反转）图T
DFS(G): G.u.f
按照G.u.f倒序计算DFS(T)
T的深度优先森林即SCC
```

### 2. Torjan Alg



## Directed Graph has a cycle？

### 1. Topological sort for dg (clrs ex 22.4-5)

重复删除入度为0的节点及其相连的节点。

如果最后没有节点剩下，则有节点删除顺序为拓扑序，无环

如果最后有节点剩下，则无法拓扑排序，有环。

```
Repeat:
	find node u : in-dgree(u) == 0
	remove (u,v)
if node left, no topological order, has cycle
```

### 2. Backedge in DFS (clrs 22.3)

对图进行DFS搜索，每个节点记录u.d发现时间，u.f完成时间。如果搜索中碰到的已搜索节点未完成，则改边为后向边，有环。

```
	// u.i: node index, u.p: u.parent, u.d: discovery time, u.f finish time
DFS(&G):
	u.d = u.f = -1
	time = 0
	for u : G.V
		DFS-VISIT(G,u)
DFS-VISIT(&G,&u)
	u.d = ++time
	visited[u.i] = true
	for v : G.Adj(u)
		if (!visited[v.i])
          v.p = u
          DFS-VISIT(G, v)
	u.f = ++time
```



# Single Source Shortest Paths

## BFS: Breadth First Search

*Input*

- Directed graph G(V,E), m=|E|, n=|V|
- weight 1 for each edge. i.e. # of edges from s to v
- source vertex s

*Alg*

- FIFO queue
- adjcent matrix

```
BFS(G, s)
	mark s as explored
	Q = FIFO queue, initialized with s
	while Q not empty:
		remove Q.front = v
		for each edge(v,w):
			if w unexplored
				mark w as explored
				add w to Q.end
```

## Dijkstra Algorithm (w6) greedy

*Input*

- Directed graph G(V,E), m=|E|, n=|V|
- non negative weight for each edge.
- source vertex s

*Alg*

- greedy chose minimum edge

```
prep:
  Edge{ v(dst node), w(wight)}
  Node{ p(parent node), s(shortest dist), v(visited)}
  Store Edges in adjacents
assist:
  SelectNearest()
  	// head for o(lgn), list for o(n)
  	return unvisited(u.v), smallest sp dist (u.s) node
  UpdateNearest(u)
  	for e in G.Adj(u) // node v from e.v  		
      if e.w + u.s < v.s // smaller sp dist
        update v.s, v.p
alg:
  init node src.s = 0, others v.s = MAX
  while ( v = SelectNearest() != -1)
    UpdateNearest(v)
sp path:
  track by u.p
```



*Runtime*

- O(mn) with array
- O(m lgn) with heap

## Bellman-Ford Algorithm (W13) DP

O(EV)

sub-problem: L[i,v] = shortest s-v path with at most i edges. (cycles are permitted.)

```
For i = 1 to n-1
	For each v in V
		A[i,v]		// in-degree(v)
```

$$A[i,v] = min\{A[i-1,v],\ min_{(w,v)\in E}\{A[i-1,w] + c_{w,v}\} \}$$

neg-detect: For all v, all future A[i,v] will be the same.

# All Pair Shortest Paths

## SSSP based

run Dijkstra or Bellman-Ford V times

## Floyd-Warshall Algorithm (w13)

O(n^3)

work with neg length

A[i,j,k]: i-j SP with in first k nodes. (cycle free)

substructure: k is in i-j path ? 

```
Init: A[i,j,0]
	0,	if i = j
	cij, if (i,j) in E
	+∞, if i!=j and (i,j) not in E
for k = 1 to n
	for i = 1 to n
		for j = 1 to n
			A[i,j,k] = min{ A[i,j,k-1], A[i,k,k-1]+A[k,j,k-1] }
```



## Johnson's Algorithm

O(mn lgn)





# Minimum Spanning Tree

## Prim's MST algorithm

O(mn) -> O(m lgn)



## Kruskal's MST algorithm



# Dynamic Programming (w12)

## KnapSack (w12)

O(nW)

A[i,x]: best solution uses only first i items & total size less than x

```
Init: A[0,x] = 0
For i = 1 to n
	For x = 0 to W
		A[i,x] = max{ A[i-1,x], A[i-1,x-wi]+vi }
```

## Sequence Alignment (w12)

O(mn)

subproblems: 1) x[m] - y[n] 2) x[m] - gap 3) gap - y[n]

A[i,j]: penalty of optimal alignment of x[i] & y[j]

```
Init: A[i,0] = A[0,i] = i * p(gap)
For i = 1 to m
	for j = 1 to m
		A[i,j]
```

$$A[i,j]=min\{A[i-1,j-1]+p_{x_i,y_j},\ A[i-1,j]+p_{gap},\ A[i,j-1]+p_{gap}\}$$

## Optimal BST (w12)

O(n^3)

Try every possible root, $$C_{i,j}$$ = cost of optimal BST for {i, i+1, ..., j-1, j}. Recurrence
$$C_{i,j}=min_{r=i,\ldots,j}\{\sum_{k=i}^jp_k + C_{i,r-1} + C_{r+1,j}\}$$

```
For s = 0 to n-1	// s=j-i, O(n)
   For i = 1 to n	// O(n)
		A[i,i+s]	// O(n)
```

  $$A[i,i+s] = min_{r=i,\ldots,i+s}\{\sum_{k=i}^{i+s}p_k + A[i,r-1] + A[r+1,i+s]\}$$

# NP-Completeness (w14)

## Vertex Cover (w14)

naive: try all possibilities, $$T = C_n^k ~ \theta(n^k)$$ 

DP: sel one edge (u,v), remove u or v, sub graph has vertex cover k-1. $$T=O(2^kE)$$

## Traveling Salesman Problem (TSP) 

