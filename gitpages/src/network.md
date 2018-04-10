---
title: network note
date: 2017-05-24 22:14:00
tags: [tag1, tag2]
---

**network note**

network note

<!--more-->

[TOC]

# 1. 网络模型

![network-NetworkModelLayers](network-NetworkModelLayers.gif)

ref: [TCP/IP Protocol Architecture](https://technet.microsoft.com/en-us/library/cc958821.aspx)

# 2. TCP



# 3. UDP

## 3.1 UDP程序流程

```
socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
int sendto  (s, pBuf, nBufLen, nFlag, sockAddr, nAddrLen);
int recvfrom(s, pBuf, nBufLen, nFlag, sockAddr, nAddrLen);

send:
	init, sendto, clean
recv:
	init, bind, recvfrom, clean
```

## 3.2 UDP报文结构

```
ip头||                    ip数据
ip头||        udp头          || udp数据
... ||源端口|目的端口|长度|校验|| ...
20B || 2B   |  2B   | 2B| 2B || 0-65507B
```

最大长度：由于UDP长度最长2字节（2^16-1），扣除头开销20+8，剩余65535-28=65507字节

MTU与分片：如果IP包长度大于MTU，则会分片为多个数据帧（由IP层实现）

超出后处理：如果UDP包长度大于65507，需要程序自己拆分，否则sendto返回长度错误（10040 WSAEMSGSIZE windows, EMSGSIZE 90 linux)

windows UDP问题：[windows下发送UDP包大于1024时速度下降](https://blog.csdn.net/wanglx_/article/details/53534804), 由于windows特性引起，可通过注册表修改