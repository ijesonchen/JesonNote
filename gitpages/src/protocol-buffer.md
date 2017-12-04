---
title: note for protocol buffer
date: 2017-05-24 22:14:00
tags: [note, protocol buffer]
---

**note for protocol buffer**

<!--more-->

[TOC]

# proto2与proto3的区别
[Protobuf 的 proto3 与 proto2 的区别](https://solicomo.com/network-dev/protobuf-proto3-vs-proto2.html)
本地镜像：
[Protobuf 的 proto3 与 proto2 的区别](proto-v2v3-difference.md)

# proto2与proto3混用

## proto版本不一致导致的编译问题

问题：
```
老项目使用proto2静态库，本地源码含有proto2头文件及lib文件。
新项目使用vcpkg安装grpc库，grpc 1.6， proto3.4.新项目grpc及proto3可以正常使用，但是导致老项目编译时提示proto版本不对（pb.cc文件）
```

解决:

```
老项目移除本地源码proto2的依赖项。直接使用vcpkg的proto3设置，使protoc3.exe生成的proto2协议文件，即可直接编译通过，正常运行。注意vcpkg默认使用动态链接。
并且，可以和其他仍然使用protoc2.exe生成的源码文件正常通信。
```

疑问：

```
protoc3和protoc2对v2消息定义生成的二进制文件，对同一个消息的序列化流程是否完全相同？
```

## 如何在同时使用proto2和proto3协议？

可能的途径（在同一个程序中实现）：

- 对protoc.exe生成的源码文件进行处理，包含不同的头文件。内部是否能够实现自动链接对应lib库？
- python可以实现吗？

如果是多个程序的系统，可以考虑引入中间接口程序及第三方协议

# proto3新特性

## json map

```
proto3支持json map
当前查到的函数包括 (namespace google::protobuf::util, json_util.h)
MessageToJsonString
JsonStringToMessage
BinaryToJsonStream
BinaryToJsonString
JsonToBinaryStream
JsonToBinaryString

即支持 
json (str) <-> message
json (str/steam) <-> binary (str/steam)
```




