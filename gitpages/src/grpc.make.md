---
title: build gprc on win10 with vs2015
date: 2017-10-05 23:14:00
tags: [grpc, build]
---
# build gprc on win10 with vs2015
how to build gprc on win10 with vs2015
<!--more-->

## build grpc proto

`protoc.exe status.proto --plugin=protoc-gen-grpc=grpc_cpp_plugin.exe --grpc_out=.`





## env

```
Windows 10 Pro 1703 15063.608
Visual Studio 2015.3 Ent
CMake 3.9.2
gRpc 1.6.3
```

## Instructions

readme.md -> install.md

install:

```
Active State Perl 5.24.2.2403
Ninja 1.8.2
Go 1.9 x64
yasm Win64 VS2010 .zip (for use with VS2010+ on 64-bit Windows), follow readme copy to vs folder
```

cmake:

```
set ASM_NASM=c:\bin\yasm-1.3.0\vsyasm.exe
// add all path needed: cmake, perl
```

git

```
grep TreatWarningAsError . -r |  awk '!a[$0]++'
sed -i "s/<TreatWarningAsError>true/<TreatWarningAsError>false/g" `grep TreatWarningAsError . -rl`
```

build