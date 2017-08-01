---
title: C / C++ note
date: 2017-05-24 22:14:00
tags: [C, C++, STL, note]
---

C/C++笔记
<!--more-->


[TOC]


# 语法

## 函数形参检查数组大小: 使用数组引用
[数组引用:C++ 数组做参数 深入分析](http://blog.csdn.net/jiangxinyu/article/details/7767065)

bool Test(int (&arr)[10]);
将会检查入参数组长度是否一致，否则编译错误

# STL

## tuple & tie

```c++
std::tuple<int, char> GetIntChar(void);

int a;
char b;
std::tie(a,b) = GetIntChar();
```

