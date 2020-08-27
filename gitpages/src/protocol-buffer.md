---
title: note for protocol buffer
date: 2017-05-24 22:14:00
tags: [note, protocol buffer]
---

**note for protocol buffer**

<!--more-->

[TOC]

# 1. PB3

## proto2与proto3的区别

[Protobuf 的 proto3 与 proto2 的区别](https://solicomo.com/network-dev/protobuf-proto3-vs-proto2.html)
本地镜像：
[Protobuf 的 proto3 与 proto2 的区别](proto-v2v3-difference.md)

## proto2与proto3混用

### proto版本不一致导致的编译问题

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

### 如何在同时使用proto2和proto3协议？

可能的途径（在同一个程序中实现）：

- 对protoc.exe生成的源码文件进行处理，包含不同的头文件。内部是否能够实现自动链接对应lib库？
- python可以实现吗？

如果是多个程序的系统，可以考虑引入中间接口程序及第三方协议

## proto3新特性

### json map

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

# 2. Protobuf in Golang

## How to

```

** protoc可以调用插件来编译proto文件。自定义的编译插件名为protoc-gen-xxx时，可以使用 protoc --xxx_out=. <proto_file> 编译。
因此，使用gogoproto的binary protoc-gen-gofast时，编译命令为
protoc --gofast_out=. myproto.proto
但是使用gogoproto高级功能时，需要在protoc指明包含路径1. 下载protoc编译器
src https://github.com/protocolbuffers/protobuf
release https://github.com/protocolbuffers/protobuf/releases
下载相应protoc-<version>-<os>-<platform>.zip
例如protoc-3.11.2-win64.zip
并解压，记下protoc运行程序位置

2. 下载golang pb插件 protoc-gen-go
GIT_TAG="v1.2.0" # change as needed
go get -d -u github.com/golang/protobuf/protoc-gen-go
git -C "$(go env GOPATH)"/src/github.com/golang/protobuf checkout $GIT_TAG
go install github.com/golang/protobuf/protoc-gen-go

3. 编译proto文件
proto header：
syntax = "proto3";
package tutorial;
import "google/protobuf/timestamp.proto";

编译命令
protoc --go_out=. xxx.proto

**NOTE1**
似乎是protoc-gen-go的一个bug，即使proto文件里面指明syntax = "proto2"，编译出来的仍然是const _ = proto.GoGoProtoPackageIsVersion3
然而gogo protobuf可以按照指定syntax生成对应的const _ = proto.GoGoProtoPackageIsVersion2/3

**NOTE2**
protoc可以调用插件来编译proto文件。自定义的编译插件名为protoc-gen-xxx时，可以使用 protoc --xxx_out=. <proto_file> 编译。
因此，使用gogoproto的binary protoc-gen-gofast时，编译命令为
protoc --gofast_out=. myproto.proto
但是使用gogoproto高级功能时，需要在protoc指明包含路径
```



## gogo  protobuf

```
1. 下载protoc编译器

2. 下载gogoproto
go get github.com/gogo/protobuf/proto
go get github.com/gogo/protobuf/{binary}
go get github.com/gogo/protobuf/gogoproto

其中{binary}包括：
protoc-gen-gofast （default proto feature）
protoc-gen-gogofast (same as gofast, but imports gogoprotobuf)
protoc-gen-gogofaster (same as gogofast, without XXX_unrecognized, less pointer fields)
protoc-gen-gogoslick (same as gogofaster, but with generated string, gostring and equal methods)
除第一个外，其他三个插件需要-I导入gogoproto包

3. 编译。如使用protoc-gen-gogofaster
protoheader示例：
syntax = "proto2";
package  testpb;

import "github.com/gogo/protobuf/gogoproto/gogo.proto";

字段后加nullable:
	repeated double Field1 = 1 [(gogoproto.nullable) = false, packed = true];

命令示例：
protoc -I=. -I=$GO1STPATH/src --gogofaster_out=.  test.proto
其中GO1STPATH为go get时使用的路径，一般是GOPATH的第一个路径(注意，gogoproto github的说明此处有误)
```





