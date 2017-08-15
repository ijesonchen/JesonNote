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


# 技巧

## 程序中自动包含代码版本及编译时间
环境：svn， vs2015
思路：
1. 代码版本：使用svn命令生成当前代码版本号并输出到文件，对文件进行处理生成相关字符串定义代码。
	svn info 获取版本状态，修改时间
    svn stat 获取文件修改/增删状态
2. 编译时间：使用编译器预处理宏定义 __DATE__ 和 __TIME__
示例：
SvnInfo2CppSrcStr 解析SVN info信息并生成相应版本定义常量代码文件。

PROPERTY SET:
	Project Property, Build Event, Pre-Build Event, Command Line, Edit
```language
	svn info $(SolutionDir) > $(SolutionDir)Bin\repoInfo.tmp
	svn stat $(SolutionDir) > $(SolutionDir)Bin\repoStat.tmp
	$(SolutionDir)doc\SvnInfo2CppSrcStr.exe $(SolutionDir)Bin\repoInfo.tmp $(SolutionDir)Bin\repoStat.tmp $(SolutionDir)repoString.tmp
```

CODE:
```language
	#include "repoString.tmp"

	stringstream ssRepo;
	ssRepo << u8"RepoInfo:" << "\r\n";
	for (auto p : g_u8RepoString)
	{
	ssRepo << "\r\n" << p;
	}

	Log(INFO, ssRepo.str());

	stringstream ssBuild;
	ssBuild << "\r\n\r\n----> Build: " << __DATE__ << ", " << __TIME__ << "\r\n";

	Log(INFO, ssBuild.str());
```

## 求二进制数有多少个1
[c语言位运算 求1个整数的二进制数有多少个1](http://blog.csdn.net/nvd11/article/details/8893207)

```language
unsigned long fun(unsigned long x)
{
    int count =  0 ;
    while(x)
    {
        count++;
        x =  x  &   (x - 1);
    }
    return count;
}
```
证明：
x = x & (x - 1)的意就是把x 化为二进制数后， 把最右边的1变成0.
最右边为1时易证。
最右边为0时，x 的二进制数是A1B，则x-1为A0C（B为n位0，C为n位1），则x&(x-1)为A0B。证毕。
时间复杂度（循环次数）为1的位数。


# STL

## tuple & tie

```c++
std::tuple<int, char> GetIntChar(void);

int a;
char b;
std::tie(a,b) = GetIntChar();
```

