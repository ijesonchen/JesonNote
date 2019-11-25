---
title: Note for Reading
date: 2019-10-12 00:00:00
tags: [note, reading]
---

# Note for Reading

Note for reading.

<!--more-->

[TOC]

# 1. 算法和数据结构

## 1.1 B树、B+树

B树

```
https://www.cnblogs.com/yangecnu/p/introduce-b-tree-and-b-plus-tree.html
	浅谈算法和数据结构: 十 平衡查找树之B树
https://zh.wikipedia.org/wiki/B树

阶：每一个节点子节点数目的最大值（Knuth）。其他人有不同的定义。
B树是一个多叉查找树，节点可以合并或分离,内部节点的key将节点的子树有序分开。
每节点内元素数量为d~2d。为2d且插入时，分裂为2个d且中间元素插入到父节点。
```

B+树

```
类似B树。区别：
1. 除了叶子结点，其他节点只放key不保存value，类似索引，更紧凑。
2. 叶子结点保存所有元素，且用链表相连。方便范围查找。
```

2-3树

```
https://zh.wikipedia.org/wiki/2-3树
每个节点最多2个元素,3个子节点有序分割
```

## 1.2 跳表

```
Skip lists are data structures that use probabilistic balancing rather than strictly enforced balancing.
插入时,随机当前key的level
操作期望是log(n)
```

# 2. 数据库

## 2.1 MySQL

## 2.2 MongoDB

## 2.3 Redis

高性能

```
单线程,避免锁竞争
epoll支持多连接
索引:跳表,实现简单,高性能,缓存区域化,方便范围查询
```

持久化

```
rdb: 快照.时间+修改次数
aof: 追加指令,磁盘同步间隔
混合
```

缓存淘汰

```
保证不超出物理内存限制
拒绝修改
有超时的key中淘汰
所有key中淘汰:
淘汰策略:lru, ttl, radom等
```

# 3. Go语言

## 3.1 GC

源码注释

```
go/src/runtime/mgc.go

// Garbage collector (GC).
//
// The GC runs concurrently with mutator threads, is type accurate (aka precise), allows multiple
// GC thread to run in parallel. It is a concurrent mark and sweep that uses a write barrier. It is
// non-generational and non-compacting. Allocation is done using size segregated per P allocation
// areas to minimize fragmentation while eliminating locks in the common case.

gc多线程并发执行，强类型。带有写屏障的并发标记-清除。非分代，非压缩。每个P分配区域有多个不同size的分区，避免碎片和锁竞争。
非分代的没有整理过程的Concurrent Mark and Sweep算法（CMS算法）

// The algorithm decomposes into several steps.
// This is a high level description of the algorithm being used. For an overview of GC a good
// place to start is Richard Jones' gchandbook.org.
//
// The algorithm's intellectual heritage includes Dijkstra's on-the-fly algorithm, see
// Edsger W. Dijkstra, Leslie Lamport, A. J. Martin, C. S. Scholten, and E. F. M. Steffens. 1978.
// On-the-fly garbage collection: an exercise in cooperation. Commun. ACM 21, 11 (November 1978),
// 966-975.
// For journal quality proofs that these steps are complete, correct, and terminate see
// Hudson, R., and Moss, J.E.B. Copying Garbage Collection without stopping the world.
// Concurrency and Computation: Practice and Experience 15(3-5), 2003.
//
// 1. GC performs sweep termination.
//
//    a. Stop the world. This causes all Ps to reach a GC safe-point.
//
//    b. Sweep any unswept spans. There will only be unswept spans if
//    this GC cycle was forced before the expected time.
1. sweep termination
  a. stop the world.所有P到达GC安全点
  b. 强制GC时,清理所有未清理区域

// 2. GC performs the mark phase.
//
//    a. Prepare for the mark phase by setting gcphase to _GCmark
//    (from _GCoff), enabling the write barrier, enabling mutator
//    assists, and enqueueing root mark jobs. No objects may be
//    scanned until all Ps have enabled the write barrier, which is
//    accomplished using STW.
启用写屏障, 

//
//    b. Start the world. From this point, GC work is done by mark
//    workers started by the scheduler and by assists performed as
//    part of allocation. The write barrier shades both the
//    overwritten pointer and the new pointer value for any pointer
//    writes (see mbarrier.go for details). Newly allocated objects
//    are immediately marked black.
//
//    c. GC performs root marking jobs. This includes scanning all
//    stacks, shading all globals, and shading any heap pointers in
//    off-heap runtime data structures. Scanning a stack stops a
//    goroutine, shades any pointers found on its stack, and then
//    resumes the goroutine.
//
//    d. GC drains the work queue of grey objects, scanning each grey
//    object to black and shading all pointers found in the object
//    (which in turn may add those pointers to the work queue).
//
//    e. Because GC work is spread across local caches, GC uses a
//    distributed termination algorithm to detect when there are no
//    more root marking jobs or grey objects (see gcMarkDone). At this
//    point, GC transitions to mark termination.
//
// 3. GC performs mark termination.
//
//    a. Stop the world.
//
//    b. Set gcphase to _GCmarktermination, and disable workers and
//    assists.
//
//    c. Perform housekeeping like flushing mcaches.
//
// 4. GC performs the sweep phase.
//
//    a. Prepare for the sweep phase by setting gcphase to _GCoff,
//    setting up sweep state and disabling the write barrier.
//
//    b. Start the world. From this point on, newly allocated objects
//    are white, and allocating sweeps spans before use if necessary.
//
//    c. GC does concurrent sweeping in the background and in response
//    to allocation. See description below.
//
// 5. When sufficient allocation has taken place, replay the sequence
// starting with 1 above. See discussion of GC rate below.

// Concurrent sweep.
//
// The sweep phase proceeds concurrently with normal program execution.
// The heap is swept span-by-span both lazily (when a goroutine needs another span)
// and concurrently in a background goroutine (this helps programs that are not CPU bound).
// At the end of STW mark termination all spans are marked as "needs sweeping".
//
// The background sweeper goroutine simply sweeps spans one-by-one.
//
// To avoid requesting more OS memory while there are unswept spans, when a
// goroutine needs another span, it first attempts to reclaim that much memory
// by sweeping. When a goroutine needs to allocate a new small-object span, it
// sweeps small-object spans for the same object size until it frees at least
// one object. When a goroutine needs to allocate large-object span from heap,
// it sweeps spans until it frees at least that many pages into heap. There is
// one case where this may not suffice: if a goroutine sweeps and frees two
// nonadjacent one-page spans to the heap, it will allocate a new two-page
// span, but there can still be other one-page unswept spans which could be
// combined into a two-page span.
//
// It's critical to ensure that no operations proceed on unswept spans (that would corrupt
// mark bits in GC bitmap). During GC all mcaches are flushed into the central cache,
// so they are empty. When a goroutine grabs a new span into mcache, it sweeps it.
// When a goroutine explicitly frees an object or sets a finalizer, it ensures that
// the span is swept (either by sweeping it, or by waiting for the concurrent sweep to finish).
// The finalizer goroutine is kicked off only when all spans are swept.
// When the next GC starts, it sweeps all not-yet-swept spans (if any).

// GC rate.
// Next GC is after we've allocated an extra amount of memory proportional to
// the amount already in use. The proportion is controlled by GOGC environment variable
// (100 by default). If GOGC=100 and we're using 4M, we'll GC again when we get to 8M
// (this mark is tracked in next_gc variable). This keeps the GC cost in linear
// proportion to the allocation cost. Adjusting GOGC just changes the linear constant
// (and also the amount of extra memory used).

// Oblets
//
// In order to prevent long pauses while scanning large objects and to
// improve parallelism, the garbage collector breaks up scan jobs for
// objects larger than maxObletBytes into "oblets" of at most
// maxObletBytes. When scanning encounters the beginning of a large
// object, it scans only the first oblet and enqueues the remaining
// oblets as new scan jobs.
```

go gc 语义(译文)

```
https://zhuanlan.zhihu.com/p/77943973
关于Golang GC的一些误解--真的比Java GC更领先吗？
包含翻译
https://www.ardanlabs.com/blog/2018/12/garbage-collection-in-go-part1-semantics.html
算法: 非分代，非压缩，并发的三色标记清理算法

1. sweep termination
2. mark
  startup - STW
  marking - concur, mark assist
3. mark termination - STW
4. sweep
  分配新内存时执行

```

三色标记: 图解Golang的GC算法

```
https://i6448038.github.io/2019/03/04/golang-garbage-collector/
1. stw 启用写屏障
2. 可达对象为灰色, 新建对象(写屏障)为灰色
3. 灰色->黑色, 灰色可达为灰色
4. 最终黑色可达,白色不可达.
```



## 3.2 携程调度



##  3.3 interface实现





# # reading list

