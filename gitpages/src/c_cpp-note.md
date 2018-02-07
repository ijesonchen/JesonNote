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

  ```
  ascii（0-0x7F): 0xxx xxxx.
  >0x80: 1...10x...x, 10xx xxxx, ..., 10xx xxxx
  	总字节数为n，则第一个byte为  n个1，0，8-n-1位数据
  	余下n-1个字节为： 10开头，6位数据
  最长编码首字节： 1111 110x。后续5个6bit，总共31bit
  ```

  ​

- C++11可以使用`wstring_convert` in `<codecvt>`


## time相关

```
1. 目前未找到线程安全的 time_t 到 tm 的方法，可以考虑平台相关函数：
windows：errno_t localtime_s(time* tm, time_t const* time)
linux:  tm *localtime_r(const time_t *timep, tm *result);

NOT THREAD-SAFE:
gmtime: tm *gmtime( const time_t *time )
localtime: tm *localtime( const time_t *time )
ctime: char* ctime( const time_t* time )
asctime: char* asctime( const std::tm* time_ptr )

THREAD-SAFE:
mktime: time_t mktime( std::tm* time )
strftime: size_t strftime( char* str, size_t count,
				 const char * format, const struct tm * time );

<iomanip>
std::put_time: template< class CharT > 
			put_time(const std::tm* tmb, const CharT* fmt);
std::get_time: template< class CharT >
			get_time( std::tm* tmb, const CharT* fmt )

// windows localtime.cpp 注释：
gmtime, _gtime64, localtime and _localtime64() all use a single
statically allocated buffer. Each call to one of these routines
destroys the contents of the previous call.
看源码,是使用了全局变量ptd->_gmtime_buffer计算tm。所以不是线程安全
2. UTC时间到localtime转换
localtime是先将time_t通过timezone偏移计算出gmt time,然后调用gmttime计算tm

未找到简单的获取timezone的方法
windows： _get_timezone()使用了全局变量 _timezone，默认初始化 +8 
可以考虑API：
	TIME_ZONE_INFORMATION   tzi;
	memset(&tzi, 0, sizeof(tzi));
	GetSystemTime(&tzi.StandardDate);
	GetTimeZoneInformation(&tzi);

简单来说，格式化时间字串未找到线程安全、标准的、跨平台的C++方法

3. pc系统time返回的是UTC的时间，转换时需要加上时区偏移计算本地时间
```



# STL

## tuple & tie

```c++
std::tuple<int, char> GetIntChar(void);

int a;
char b;
std::tie(a,b) = GetIntChar();
```

##  hash

vs2015/2017 使用FNV哈希算法

据称，[C++ 下一代标准库 tr1中默认的哈希 FNV hash](http://www.cnblogs.com/napoleon_liu/archive/2010/12/26/1917396.html)

# Visual Studio

## versions

- visual studio release name
- msbuild version
- Visual C++ compiler version (MSVC++), cl.exe
- linker version, link.exe
- visual sutdio platform toolset version

| release  | CL    | msbuild | link  | toolset |
| -------- | ----- | ------- | :---- | ------- |
| vc6      | 12.00 |         |       | 8.0     |
| .net 02  | 13.00 |         |       | 9.0     |
| .net 03  | 13.10 |         |       | 10.0    |
| vs05     | 14.00 |         |       | 8.0     |
| vs08 sp1 | 15.00 | ?       |       | 9.0     |
| vs10 sp1 | 16.00 |         |       | 10.0    |
| vs12     | 17.00 |         | 11.00 | 11.0    |
| vs13 u5  | 18.00 | 12.0    | 12.00 | 12.0    |
| vs15 u3  | 19.00 | 14.0    | 14.00 | 14.0    |
| vs17 u3  | 19.11 | 15.3    | 14.11 | 14.1    |

## vcpkg

安装：git clone然后按照说明运行即可。安装过程中需要下载cmake并复制到指定目录，注意不要点错取消。

搜索包：`vcpkg search sqlite`

包支持的平台triplet:`vcpkg help triplet`

安装: `vcpkg install tbb:x64-windows` （默认x86平台)

集成：`vcpkg integrate install`

