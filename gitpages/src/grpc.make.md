---
title: build gprc on win10 with vs2015
date: 2017-10-05 23:14:00
tags: [grpc, build]
---
**How to build and use gRpc**

How to build gRpc on win10 with vs2015, and some example.

[Quick Starts](https://grpc.io/docs/quickstart):  code for a simple gRpc deom to show how it works.

<!--more-->

# Build gRpc on win10 with vs2015

how to build gprc on win10 with vs2015


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

# Quick Start

[Quick Start Guide](https://grpc.io/docs/quickstart/cpp.html) : This guide gets you started with gRPC in C++ with a simple working example.


## What’s next

- Read a full explanation of how gRPC works in [What is gRPC?](https://grpc.io/docs/guides/) and [gRPC Concepts](https://grpc.io/docs/guides/concepts.html)
- Work through a more detailed tutorial in [gRPC Basics: C++](https://grpc.io/docs/tutorials/basic/c.html)
- Explore the gRPC C++ core API in its [reference documentation](https://grpc.io/grpc/cpp/)

# [Guides](https://grpc.io/docs/guides/)



### [What is gRPC?](https://grpc.io/docs/guides/index.html)

A service with methods can be called remotely with parameters & return types. gRPC can use protocol buffers as both its IDL and as its underlying message nterchange format. Proto3 recommended.

### [gRPC Concepts](https://grpc.io/docs/guides/concepts.html)

With optional stream qualifier, can define four kinds of service method: unary , server / client streaming, bidirectional stream RPC.

Generate client- and server-side code (framework) with protocol buffer compiler plugins. Support both synchronous and asynchronous flavors in most languages.

Other features: deadline or timeouts, termination, cancelling, metadata, channels.

### [Authentication](https://grpc.io/docs/guides/auth.html)

Mechanisms: SSL/TSL, token-based authentication with Google. 

Credential can attached to a Channel or Call (ClientContext).

### [Wire format](https://grpc.io/docs/guides/wire.html)

A detailed description for an implementation of gRPC carried over [HTTP2 framing](https://tools.ietf.org/html/rfc7540)

### [Error handling and debugging](https://grpc.io/docs/guides/error.html)

How gRPC deals with errors, including gRPC’s built-in error codes.

### [Benchmarking](https://grpc.io/docs/guides/benchmarking.html)




# Tutorials

[gRPC Basics - C++](https://grpc.io/docs/tutorials/basic/c.html): A tutorial that steps you through creating a simple gRPC C++ example application.

How to:

- Define a service in a .proto file.
- Generate server and client code using the protocol buffer compiler.
- Use the C++ gRPC API to write a simple client and server for your service.

References：

- [Overview](https://grpc.io/docs/index.html)
- [protocol buffers](https://developers.google.com/protocol-buffers/docs/overview)
- [proto3 language guide](https://developers.google.com/protocol-buffers/docs/proto3) 
- [C++ generated code guide](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated)
- [protobuf release notes](https://github.com/google/protobuf/releases)

## Why use gRPC?

- Define service once in a .proto file 
- Implement in many language
- Get advantage of protocol buffers.

## Example code and setup

[grpc/grpc/examples/cpp/route_guide](https://github.com/grpc/grpc/tree/v1.6.x/examples/cpp/route_guide)

[the C++ quick start guide](https://grpc.io/docs/quickstart/cpp.html)

## Defining the service

<u>***NOTE***</u>

keywords: service, rpc, returns, stream

rpc type: simple, server-side / client-side / bidirectional streaming

```protobuf
syntax = "proto3";
message Point { ... }
message Feature { ... }
...
service RouteGuide
{
  rpc GetFeature(Point) returns (Feature) {}
  rpc ListFeatures(Rectangle) returns (stream Feature) {}
  rpc RecordRoute(stream Point) returns (RouteSummary) {}
  rpc RouteChat(stream RouteNote) returns (stream RouteNote) {}
}
```

## Generating client and server code

With predefined [makefile](https://github.com/grpc/grpc/blob/v1.6.x/examples/cpp/route_guide/Makefile) generate message & sevice interface and implement class.

```
$ make route_guide.grpc.pb.cc route_guide.pb.cc
```

which actually runs:

```
$ protoc -I ../../protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../../protos/route_guide.proto
$ protoc -I ../../protos --cpp_out=. ../../protos/route_guide.proto
```

## Creating the server

[examples/cpp/route_guide/route_guide_server.cc](https://github.com/grpc/grpc/blob/v1.6.x/examples/cpp/route_guide/route_guide_server.cc)

### Implementing RouteGuide

`RouteGuideImpl` class for interface implementation inherit from class `RouteGuide::Service` (*syncronous* version) for  `RouteGuid::AsynService` (typedef asynchronous version)

```
class RouteGuideImpl final : public RouteGuide::Service 
{
public:
	RouteGuideImpl( /*init param*/ ) { /*init Code*/ };
}
```

Implements all the service methods, make sure they are thread safe, synchronized access may be required. 

```
Status GetFeature(ServerContext* context, const Point* point,
                  Feature* feature) override {
  // set feature according to point
  return Status::OK;
}
```

For server streaming RPC, use ServerWriter::Write to send each response.

```
Status ListFeatures(ServerContext* context, const Rectangle* rectangle,
                    ServerWriter<Feature>* writer) override {
  // feature list according to rectangle
  // writer->Write each feature
  return Status::OK;
}
```

For client streaming RPC, use ServerReader::Read to traverse data.

```
while (stream->Read(&point)) {
  ...//process client input
}
```

For bidirectional streaming, use ServerReaderWriter::Read/Write to process data.

```
Status RouteChat(ServerContext* context,
                 ServerReaderWriter<RouteNote, RouteNote>* stream) override {
	// stream->Read ... stream->Write ... 
	// in one stream, messaged read in write order
	// server / client stream are independent, so both side can op in any order
	return Status::OK;
}
```

### Starting the server

- ServerBuilder::AddListeningPort(addr_uri, credential)
- ServerBuilder::RegisterService( pService)
- ServerBuilder::BuildAndStart() -> Server
- Server::Wait()     // block
- Server::Shutdown()

```
void RunServer(const std::string& db_path) {
  std::string server_address("0.0.0.0:50051");
  RouteGuideImpl service(db_path);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}
```

with thread:

```
void ServerThread(/* server param*/, std::unique_ptr<Server>* pServer) 
{
	// start server and wait
}

void ServerMain(void)
{
	std::unique_ptr<Server>& server;
	std::thread th(ServerThread, /*param*/, &server);
	// wait signal for shutdown
	if (server) { server->Shutdown(); }
	th.join();
}
```



## Creating the client

[examples/cpp/route_guide/route_guide_client.cc](https://github.com/grpc/grpc/blob/v1.6.x/examples/cpp/route_guide/route_guide_client.cc).

### Creating a stub

To call service methods, we first need to create a *stub*.

First we need to create a gRPC *channel* for our stub, specifying the server address and port we want to connect to and any special channel arguments - in our case we’ll use the default `ChannelArguments` and no SSL:

```
grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials(), ChannelArguments());

```

Now we can use the channel to create our stub using the `NewStub` method provided in the `RouteGuide` class we generated from our .proto.

```
public:
 RouteGuideClient(std::shared_ptr<ChannelInterface> channel,
                  const std::string& db)
     : stub_(RouteGuide::NewStub(channel)) {
   ...
 }

```

### Calling service methods

Now let’s look at how we call our service methods. Note that in this tutorial we’re calling the *blocking/synchronous* versions of each method: this means that the RPC call waits for the server to respond, and will either return a response or raise an exception.

#### Simple RPC

Calling the simple RPC `GetFeature` is nearly as straightforward as calling a local method.

```
Point point;
Feature feature;
point = MakePoint(409146138, -746188906);
GetOneFeature(point, &feature);

...

bool GetOneFeature(const Point& point, Feature* feature) {
  ClientContext context;
  Status status = stub_->GetFeature(&context, point, feature);
  ...
}

```

As you can see, we create and populate a request protocol buffer object (in our case `Point`), and create a response protocol buffer object for the server to fill in. We also create a `ClientContext`object for our call - you can optionally set RPC configuration values on this object, such as deadlines, though for now we’ll use the default settings. Note that you cannot reuse this object between calls. Finally, we call the method on the stub, passing it the context, request, and response. If the method returns `OK`, then we can read the response information from the server from our response object.

```
std::cout << "Found feature called " << feature->name()  << " at "
          << feature->location().latitude()/kCoordFactor_ << ", "
          << feature->location().longitude()/kCoordFactor_ << std::endl;

```

#### Streaming RPCs

Now let’s look at our streaming methods. If you’ve already read [Creating the server](https://grpc.io/docs/tutorials/basic/c.html#server) some of this may look very familiar - streaming RPCs are implemented in a similar way on both sides. Here’s where we call the server-side streaming method `ListFeatures`, which returns a stream of geographical`Feature`s:

```
std::unique_ptr<ClientReader<Feature> > reader(
    stub_->ListFeatures(&context, rect));
while (reader->Read(&feature)) {
  std::cout << "Found feature called "
            << feature.name() << " at "
            << feature.location().latitude()/kCoordFactor_ << ", "
            << feature.location().longitude()/kCoordFactor_ << std::endl;
}
Status status = reader->Finish();

```

Instead of passing the method a context, request, and response, we pass it a context and request and get a `ClientReader` object back. The client can use the `ClientReader` to read the server’s responses. We use the `ClientReader`s `Read()` method to repeatedly read in the server’s responses to a response protocol buffer object (in this case a `Feature`) until there are no more messages: the client needs to check the return value of `Read()` after each call. If `true`, the stream is still good and it can continue reading; if `false` the message stream has ended. Finally, we call `Finish()` on the stream to complete the call and get our RPC status.

The client-side streaming method `RecordRoute` is similar, except there we pass the method a context and response object and get back a `ClientWriter`.

```
std::unique_ptr<ClientWriter<Point> > writer(
    stub_->RecordRoute(&context, &stats));
for (int i = 0; i < kPoints; i++) {
  const Feature& f = feature_list_[feature_distribution(generator)];
  std::cout << "Visiting point "
            << f.location().latitude()/kCoordFactor_ << ", "
            << f.location().longitude()/kCoordFactor_ << std::endl;
  if (!writer->Write(f.location())) {
    // Broken stream.
    break;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(
      delay_distribution(generator)));
}
writer->WritesDone();
Status status = writer->Finish();
if (status.IsOk()) {
  std::cout << "Finished trip with " << stats.point_count() << " points\n"
            << "Passed " << stats.feature_count() << " features\n"
            << "Travelled " << stats.distance() << " meters\n"
            << "It took " << stats.elapsed_time() << " seconds"
            << std::endl;
} else {
  std::cout << "RecordRoute rpc failed." << std::endl;
}

```

Once we’ve finished writing our client’s requests to the stream using `Write()`, we need to call `WritesDone()` on the stream to let gRPC know that we’ve finished writing, then `Finish()` to complete the call and get our RPC status. If the status is `OK`, our response object that we initially passed to `RecordRoute()` will be populated with the server’s response.

Finally, let’s look at our bidirectional streaming RPC `RouteChat()`. In this case, we just pass a context to the method and get back a `ClientReaderWriter`, which we can use to both write and read messages.

```
std::shared_ptr<ClientReaderWriter<RouteNote, RouteNote> > stream(
    stub_->RouteChat(&context));
```

The syntax for reading and writing here is exactly the same as for our client-streaming and server-streaming methods. Although each side will always get the other’s messages in the order they were written, both the client and server can read and write in any order — the streams operate completely independently.

## Try it out!

Build client and server:

```
$ make

```

Run the server, which will listen on port 50051:

```
$ ./route_guide_server
```

Run the client (in a different terminal):

```
$ ./route_guide_client
```
## QA

### Q: fatal error C1189

```
1>c:\svn\grpctest\public\grpc\impl\codegen\port_platform.h(44): fatal error C1189: #error:      "Please compile grpc with _WIN32_WINNT of at least 0x600 (aka Windows Vista)"
```

**A: define: /D_WIN32_WINNT=0x600**

项目属性，C/C++， 命令行，添加 /D_WIN32_WINNT=0x600

### Q: error LNK2019

```
1>gRpcTest.obj : error LNK2019: 无法解析的外部符号 "public: virtual __cdecl grpc::Server::~Server(void)" (??1Server@grpc@@UEAA@XZ)，该符号在函数 "public: virtual void * __cdecl grpc::Server::`scalar deleting destructor'(unsigned int)" (??_GServer@grpc@@UEAAPEAXI@Z) 中被引用
```

**A: link libs**

for debug:
```
Ws2_32.lib
crypto.lib 
gpr.lib
grpc++.lib
grpc.lib
libprotobufd.lib
ssl.lib
zlibd.dll
zlibd.lib
```

**How To find related lib:**

- search symbol test from all grpc lib
- dumpbin all grpc lib


example of `dumpbin grpc++.lib /symbols`


```
157E 00000A20 SECT6  notype ()    External     | ??1Server@grpc@@UEAA@XZ (public: virtual __cdecl grpc::Server::~Server(void))

12A2 00000000 UNDEF  notype ()    External     | ??1Server@grpc@@UEAA@XZ (public: virtual __cdecl grpc::Server::~Server(void))
```


[ref: DUMPBIN /symbols](https://msdn.microsoft.com/en-us/library/b842y285.aspx)
```
If the third column contains SECT*x*, the symbol is defined in that section of the object file. But if UNDEF appears, it is not defined in that object and must be resolved elsewhere.  
  
The fifth column (Static, External) tells whether the symbol is visible only within that object, or whether it is public (visible externally). A Static symbol, _sym, wouldn't be linked to a Public symbol _sym; these would be two different instances of functions named _sym. 

The last column in a numbered line is the symbol name, both decorated and undecorated.  
```
```
SECTxx 表示定义的代码段
UNDEF  表示链接外部代码
External 表示暴露的接口
Static	表示仅内部使用
```

[ref: DbgHelp Reference](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679292(v=vs.85).aspx)

Windows desktop applications -> Develop -> Desktop technologies -> Diagnostics -> Debugging and Error Handling -> DbgHelp Reference -> Image Help Library -> 

[ref: DumpBin.exe的实现原理](http://blog.csdn.net/lostspeed/article/details/54647761)

```
_spawnvp调用了CreateProcessA.

dumpbin 调用link.exe来实现dump的功能
link.exe /dump /all Z:\chapter1\1.1\asm1.exe
dumpbin /all Z:\chapter1\1.1\asm1.exe
```



