---
title: Protobuf 的 proto3 与 proto2 的区别
date: 2017-12-01 00:00:00
tags: [protobuf, note]
---


# [Protobuf 的 proto3 与 proto2 的区别](https://solicomo.com/network-dev/protobuf-proto3-vs-proto2.html)

On 2015-07-17 19:16:00 By Soli

这是一篇学习笔记。在粗略的看了 [Protobuf 的文档](https://developers.google.com/protocol-buffers/docs/overview)中关于 [proto2](https://developers.google.com/protocol-buffers/docs/proto) 和 [proto3](https://developers.google.com/protocol-buffers/docs/proto3) 的说明后，记录下了几点 proto3 区别于 proto2 的地方。

<!--more-->

总的来说，proto3 比 proto2 支持更多语言但 更简洁。去掉了一些复杂的语法和特性，更强调约定而弱化语法。如果是首次使用 Protobuf ，建议使用 proto3 。

1. 在第一行非空白非注释行，必须写：

   syntax = "proto3";

2. 字段规则移除了 “required”，并把 “optional” 改名为 “singular”[1](https://solicomo.com/network-dev/protobuf-proto3-vs-proto2.html#fn:1)；

   在 proto2 中 required 也是不推荐使用的。proto3 直接从语法层面上移除了 required 规则。其实可以做的更彻底，把所有字段规则描述都撤销，原来的 repeated 改为在类型或字段名后加一对中括号。这样是不是更简洁？

3. “repeated”字段默认采用 `packed` 编码；

   在 proto2 中，需要明确使用 `[packed=true]` 来为字段指定比较紧凑的 `packed` 编码方式。

4. 语言增加 Go、Ruby、JavaNano 支持；

5. 移除了 default 选项；

   在 proto2 中，可以使用 default 选项为某一字段指定默认值。在 proto3 中，字段的默认值只能根据字段类型由系统决定。也就是说，默认值全部是约定好的，而不再提供指定默认值的语法。

   在字段被设置为默认值的时候，该字段不会被序列化。这样可以节省空间，提高效率。

   但这样就无法区分某字段是根本没赋值，还是赋值了默认值。这在 proto3 中问题不大，但在 proto2 中会有问题。

   比如，在更新协议的时候使用 default 选项为某个字段指定了一个与原来不同的默认值，旧代码获取到的该字段的值会与新代码不一样。

   另一个重约定而弱语法的例子是 Go 语言里的公共/私有对象。Go 语言约定，首字母大写的为公共对象，否则为私有对象。所以在 Go 语言中是没有 public、private 这样的语法的。

6. 枚举类型的第一个字段必须为 0 ；

   这也是一个约定。

7. 移除了对分组的支持；

   分组的功能完全可以用消息嵌套的方式来实现，并且更清晰。在 proto2 中已经把分组语法标注为『过期』了。这次也算清理垃圾了。

8. 旧代码在解析新增字段时，会把不认识的字段丢弃，再序列化后新增的字段就没了；

   在 proto2 中，旧代码虽然会忽视不认识的新增字段，但并不会将其丢弃，再序列化的时候那些字段会被原样保留。

   我觉得还是 proto2 的处理方式更好一些。能尽量保持兼容性和扩展能力，或许实现起来也更简单。proto3 现在的处理方式，没有带来明显的好处，但丢掉了部分兼容性和灵活性。

   **[2017-06-15 更新]**：经过漫长的[讨论](https://github.com/google/protobuf/issues/272)，官方终于同意在 proto3 中恢复 proto2 的处理方式了。 可以通过[这个文档](https://docs.google.com/document/d/1KMRX-G91Aa-Y2FkEaHeeviLRRNblgIahbsk4wA14gRk/view)了解前因后果及时间线。

9. 移除了对扩展的支持，新增了 Any 类型；

   Any 类型是用来替代 proto2 中的扩展的。目前还在开发中。

   proto2 中的扩展特性很像 Swift 语言中的扩展。理解起来有点困难，使用起来更是会带来不少混乱。

   相比之下，proto3 中新增的 Any 类型有点像 C/C++ 中的 void* ，好理解，使用起来逻辑也更清晰。

10. 增加了 JSON 映射特性；

    语言的活力来自于与时俱进。当前，JSON 的流行有其充分的理由。很多『现代化』的语言都内置了对 JSON 的支持，比如 Go、PHP 等。而 C++ 这种看似保罗万象的学院派语言，因循守旧、故步自封，以致于现出了式微的苗头。

------

1. 实际上，这样说并不准确，但这样理解起来更简单。 [↩︎](https://solicomo.com/network-dev/protobuf-proto3-vs-proto2.html#fnref:1)