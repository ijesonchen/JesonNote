---
title: Note for PAT test
date: 2017-05-24 22:14:00
tags: [alg, note, shortest path]
---

#  PAT test Note

PAT test note.

<!--more-->

# NOTE

## redirect file to cin

```
ifstream ifinput("filename.txt");
cin.rdbuf(ifinput.rdbuf());
// ifinput must exist when use cin
```



# BASIC

# ADVANCED

## A1002

使用map记录幂n和系数c

*系数c可能为0，需排除*

## A1003

考虑使用Dijkstra最短路径

但是，要记录 最短路径个数，以及沿途team数量，考虑动态规划



# TOP

