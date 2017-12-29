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

## 显式类型转换

C++11 以后开始使用{}语法实现显示类型转换。

[C++/C++ language/Explicit type conversion](http://en.cppreference.com/w/cpp/language/expressions)

Converts between types using a combination of explicit and implicit conversions.

| Syntax                                 | tag  | note          |
| -------------------------------------- | ---- | ------------- |
| ( new_type ) expression                | (1)  |               |
| new_type ( expression )                | (2)  |               |
| new_type ( expressions )               | (3)  |               |
| new_type ( )                           | (4)  |               |
| new_type { expression-list(optional) } | (5)  | (since C++11) |
| template ( expressions(optional) )     | (6)  | (since C++17) |
| template { expressions(optional) }     | (7)  | (since C++17) |

Returns a value of type `new_type`.


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

# C/C++库

## locale相关

- setlocale设置全局locale，影响cout，mbstowcs，isdigt等函数
- mbstowcs/wcstombs使用全局locale
- isdigit in `<cctype>`使用全局locale
- std::isdigit in `<locale>`带带有locale参数，每次使用指定的locale
- locale字符/代码页是平台相关的
- ANSI转换windows平台建议使用`WideCharToMultiByte/MultiByteToWideChar`， linux平台可以考虑`iconv`
- utf8转换除了上述函数外，可以考虑手写UTF编码解析函数
- C++11可以使用`wstring_convert` in `<codecvt>`




# STL

## tuple & tie

```c++
std::tuple<int, char> GetIntChar(void);

int a;
char b;
std::tie(a,b) = GetIntChar();
```

# Visual Studio

## versions

- visual studio release name
- msbuild version
- Visual C++ compiler version (MSVC++), cl.exe
- linker version, link.exe
- visual sutdio platform toolset version

| release  | msbuild | CL    | link  | toolset |
| -------- | ------- | ----- | :---- | ------- |
| vc6      |         | 12.00 |       | 8.0     |
| .net 02  |         | 13.00 |       | 9.0     |
| .net 03  |         | 13.10 |       | 10.0    |
| vs05     |         | 14.00 |       | 8.0     |
| vs08 sp1 | ?       | 15.00 |       | 9.0     |
| vs10 sp1 |         | 16.00 |       | 10.0    |
| vs12     |         | 17.00 | 11.00 | 11.0    |
| vs13 u5  | 12.0    | 18.00 | 12.00 | 12.0    |
| vs15 u3  | 14.0    | 19.00 | 14.00 | 14.0    |
| vs17 u3  | 15.3    | 19.11 | 14.11 | 14.1    |

## vcpkg

安装：git clone然后按照说明运行即可。安装过程中需要下载cmake并复制到指定目录，注意不要点错取消。

搜索包：`vcpkg search sqlite`

包支持的平台triplet:`vcpkg help triplet`

安装: `vcpkg install tbb:x64-windows` （默认x86平台)

集成：`vcpkg integrate install`

