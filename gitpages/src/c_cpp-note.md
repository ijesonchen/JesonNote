---
title: C / C++ note
date: 2017-05-24 22:14:00
tags: [C, C++, STL, note]
---

C/C++笔记
<!--more-->


[TOC]


# 1 语法

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


# 2 技巧

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

# 3 C/C++库

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



# 4 STL

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

# 5 linux

## gcc/g++

```##
linux 安装指定版本
centos 7.5
yum install gcc gcc-c++ // 4.8.5 位置 /bin/gcc /bin/g++
yum install devtoolset-3-gcc devtoolset-3-gcc-c++ // 4.9 toolset-9 9.0
scl enable devtoolset-3 zsh // 启动zsh，并且使用toolset3，exit后失效。
直接指定变量：export CC=/opt/rh/devtoolset-3/root/usr/bin/gcc
source /opt/rh/devtoolset-3/enable

**** 安装devtoolset-8
https://www.softwarecollections.org/en/scls/rhscl/devtoolset-8/
# 1. Install a package with repository for your system:
# On CentOS, install package centos-release-scl available in CentOS repository:
$ sudo yum install centos-release-scl

# On RHEL, enable RHSCL repository for you system:
$ sudo yum-config-manager --enable rhel-server-rhscl-7-rpms

# 2. Install the collection:
$ sudo yum install devtoolset-8

# 3. Start using software collections:
$ scl enable devtoolset-8 bash

ubuntu 18.04
apt install gcc-4.8 g++-4.8 // 4.8.2 位置：/usr/bin/gcc-4.8

多个版本如果没有覆盖文件的话，可以通过设置环境变量修改
export CC=/bin/gcc
export CXX=/bin/g++

也可以考虑下载rpm、deb包，或源码编译

提示找不到devtoolset-8-gcc-c++时：
yum install centos-release-scl
yum install devtoolset-8-gcc-c++

ubuntu 1804安装gcc-10
https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test
add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt install g++-10 gcc-10
```

### 参数

```
-L=./lib // 静态库搜索目录
-lxxx    // 链接 libxxx.a / libxxx.so
-O3      // 优化级别 O0禁用优化
-std=c++11 // 启用C++11
-fPIC    // 生成静态库
-I <dir>  // include目录
-g // debug信息

全局变量
LD_LIBRARY_PATH=<lib_path> 
```



### 编译示例

```
编译静态库 .a
g++ -std=c++11 -DFINTEGER=int  -fopenmp  -I. -fPIC -m64 -Wno-sign-compare -g -O3 -Wall -Wextra -mpopcnt -msse4 -c cfaisslib/cfaiss.cpp -o cfaisslib/cfaiss.o -lopenblas -lblas -lcfaiss -lfaiss
ar r libcfaiss.a cfaisslib/cfaiss.o
链接静态库和faiss
g++ -std=c++11 -DFINTEGER=int  -fopenmp  -I. -m64 -Wno-sign-compare -g -O3 -Wall -Wextra -mpopcnt -msse4 -o main main.cpp -fopenmp -L=./third_part -lopenblas -lblas -lcfaiss   -lfaiss
```

### 运行时越界检查

```
https://github.com/google/sanitizers/wiki/AddressSanitizer
https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
https://blog.csdn.net/weixin_41644391/article/details/103450401
https://www.jianshu.com/p/3a2df9b7c353

gcc clang集成，编译开关打开即可。lib库、camke，bazel都可以支持。
测试代码：
  auto ppp = new int[10];
  LOG(INFO) << "==========> try overflow";
  LOG(INFO) << "==========> overflow " << ppp[10];
运行会有提示并打印调用堆栈
==14158==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60400001c7b8 at pc 0x000000557af2 bp 0x7fffa68771b0 sp 0x7fffa68771a0
READ of size 4 at 0x60400001c7b8 thread T0

cmake：
set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -DNDEBUG -O2 -D__const__= -pipe -g -W -Wall -Wextra -Wno-unused-parameter -fPIC -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBRPC_ENABLE_CPU_PROFILER -lglog  -fno-omit-frame-pointer  -fsanitize=address")
target_link_libraries(qiao_brpc ${BRPC_LIB} ${DYNAMIC_LIB} ${GPERFTOOLS_LIBRARIES} -fno-omit-frame-pointer -fsanitize=address)

bazel:
https://github.com/bazelment/trunk/blob/master/tools/bazel.rc
（0.25修改文件.bazelrc)
修改bazelrc文件：
build:asan --crosstool_top //tools/lrte:toolchain
build:asan --compiler clang
build:asan --strip=never
build:asan --copt -fsanitize=address
build:asan --copt -DADDRESS_SANITIZER
build:asan --copt -O1
build:asan --copt -g
build:asan --copt -fno-omit-frame-pointer
build:asan --linkopt -fsanitize=address
编译：
bazel build --config asan
```

### 临时忽略指定错误/警告

```
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic pop
```



## gdb

```
yum install gdb
```

### 常用指令

```
https://linuxtools-rst.readthedocs.io/zh_CN/latest/tool/gdb.html   常用指令
https://www.cnblogs.com/rosesmall/archive/2012/04/10/2440514.html  带参数调试

****启动程序
gdb <bin>
set args <bin-args>

****attach
gdb attach `pidof <prog-name>`
gdb -c xxx.core  <bin-name>

thread apply all bt // 打印所有线程调用栈

file xxx // 加载符号

bt // backtrace打印调用栈
t n // 切换到线程n
f n // 切换到调用栈第n帧
p symbol // 打印symbol的值
https://blog.csdn.net/counsellor/article/details/87087871
info variables // 显示variales信息
info locals // 显示当前帧变量
info args // 显示当前帧参数
x/16 &var // 显示var地址对应的数据，16单位，详细 help x

list // 显示源码。如果找不到no such file or directory，则添加源码搜索目录
show dir // 显示工作目录. 默认$cdir(编译目录) $cwd(当前目录)
dir <source-dir> // 添加源码搜索目录
info source // 源码信息
```

###  示例

```
例如：test --args xxx
gdb test
set args --args xxx // 设置运行参数
show args // 查看
r // run 开始运行
ctrl-c // 暂停执行
l main.cpp:20 // list 显示main.cpp第20行
b main.cpp:20 // break 设置断点
c // continue
n // next
```





### pstack 

```
pstack工具是一个gdb脚本，使用gdb的bt或者thread apply all bt命令，通过sed工具输出线程堆栈信息
https://nanxiao.me/linux-pstack/
```

### strace

```

strace -fp <pid> -s<截断长度> -e trace=write
链接到pid的write函数，即现实其他线程输出。
```

### elfutils

```
https://sourceware.org/elfutils/
查看elf文件、进程或者coredump的backtrace等信息
```



## cmake

ref

```
https://blog.csdn.net/zhuiyunzhugang/article/details/88142908 超详细的cmake教程
https://cmake.org/cmake/help/latest/guide/tutorial/index.html CMake Tutorial

ubuntu的curl头文件目录不在/usr/lib下
LIBRARY_PATH=/usr/lib/x86_64-linux-gnu build.sh
```

## bazel

```
google开源构建工具
自动从网络下载依赖并编译**// 网络不好容易挂
bazel build <project-name>
bazel test <xxx>

目录结构：会生成目录 ${HOME}/.cache/bazel/_bazel_root/
execroot/<workspace_name>/

bazel test -c dbg //framework:value_model_test --test_output=all
log：execroot/lambda/bazel-out/k8-dbg/testlogs/framework/value_model_test/test.log
bin：execroot/lambda/bazel-out/k8-dbg/bin/framework/value_model_test.runfiles/lambda/framework/value_model_test
core: execroot/lambda/bazel-out/k8-dbg/bin/framework/value_model_test.runfiles/lambda/core.xxx
```

## 链接

```
https://blog.csdn.net/HopingWhite/article/details/7208661 linux如何查找.so
LD_LIBRARY_PATH：全局变量，在哪些目录中可以找到共享库。可以设置多个搜索目录，这些目录之间用冒号分隔开。

```

### 相关命令

```

ldd <bin> // 查看so依赖
lsof -p `pidof <bin>` |grep "\.so" // 查看程序运行时使用的so。符号链接会被解析为实际文件
```



# 6 VS Code

ref

```
https://code.visualstudio.com/docs/remote/remote-overview VS Code Remote Development
```

## 常用配置

```
编辑预览：跳转到一个文件时，默认预览模式，不保留窗口（标题栏为斜体），双击后变为普通打开模式。可以考虑false
"workbench.editor.enablePreview": true, 
"workbench.editor.enablePreviewFromQuickOpen": true,

https://code.visualstudio.com/docs/remote/troubleshooting
启动环境变量 .bash_rc 可配置htts_proxy
```





## sftp

```
1. 安装sftp插件（liximomo.sftp）
  可以安装在本地，或者wsl中的remote server中
  支持本地同步到远程，或者通过远程跳转到另一个服务器。
2. 配置示例
{
    "name": "conf-name",
    "host": "ip/srv-name",
    "protocol": "sftp",
    "port": 22,
    "username": "root",
    "privateKeyPath":"<id_rsa-file-full-path>",
    "remotePath": "<remote-path>",
    "uploadOnSave": true
}
可以使用命令 sftp -P <port> user@host 测试连接是否正常。
3. 配置成功后，会有一个左侧导航栏，显示配置实例，并且可以浏览文件。
4. 右键文件或文件夹，可以执行同步 Sync xxx
```

## Remote Development

```
https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack
vscode 远程开发套件，包括ssh, wsl, container。
remote ssh: 通过跳板机登录：利用ProxyCommand。
	使用了ssh_config配置，参考：
	https://linux.die.net/man/5/ssh_config
	https://code.visualstudio.com/blogs/2019/10/03/remote-ssh-tips-and-tricks
	https://cikeblog.com/proxycommand.html
	
	
示例：
Host red-dev
  HostName 10.4.40.222
  User root
  ForwardAgent yes
  PermitLocalCommand yes
  Port 22222

Host red-dev-docker
  HostName 127.0.0.1
  User root
  ForwardAgent yes
  PermitLocalCommand yes
  Port 222
  // %h:%p  127.0.0.1:222。
  ProxyCommand ssh.exe -q -W %h:%p red-dev 
** 注意，ProxyCommand中必须是ssh.exe，否则会报错：
CreateProcessW failed error:2
posix_spawn: No such file or directory
> 过程试图写入的管道不存在。
```

## 调试

```

```



### coredump

```
https://code.visualstudio.com/docs/cpp/cpp-debug#_memory-dump-debugging
https://stackoverflow.com/questions/55736235/how-do-you-debug-a-linux-core-dump-using-vscode
{
            "name": "core lambda service",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/lambda-service/bin/LambdaServer",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],     
            "sourceFileMap": {
                "/codebase/serving": "/codebase/lambda-service"
            },
            "coreDumpPath": "${workspaceFolder}/lambda-service/bin/core.LambdaServer.13275"
        },
        

sourceFileMap应该可以映射源码，但似乎有问题（？？？）
call stack查找对应源码时查找 /proc/self/cwd/xxx ，一般映射到用户目录。可以通过 ln -s 映射。注意点击堆栈时vscode给出的找不到文件的提示，可以点create file后查找文件创建位置。
```





# 7 Visual Studio

## 7.1 versions

- visual studio release name
- msbuild version
- Visual C++ compiler version (MSVC++), cl.exe
- linker version, link.exe
- visual sutdio platform toolset version

| release     | CL    | msbuild | link  | toolset |
| ----------- | ----- | ------- | :---- | ------- |
| vc6         | 12.00 |         |       | 8.0     |
| .net 02     | 13.00 |         |       | 9.0     |
| .net 03     | 13.10 |         |       | 10.0    |
| vs05        | 14.00 |         |       | 8.0     |
| vs08 sp1    | 15.00 | ?       |       | 9.0     |
| vs10 sp1    | 16.00 |         |       | 10.0    |
| vs12        | 17.00 |         | 11.00 | 11.0    |
| vs13 u5     | 18.00 | 12.0    | 12.00 | 12.0    |
| vs15 u3     | 19.00 | 14.0    | 14.00 | 14.0    |
| vs17 u3     | 19.11 | 15.3    | 14.11 | 14.1    |
| vs19 16.4.5 | 19.24 | 16.4.5  | 14.24 |         |

## 7.2 vcpkg

安装使用

```
安装：git clone然后按照说明运行即可。安装过程中需要下载cmake并复制到指定目录，注意不要点错取消。
搜索包：`vcpkg search sqlite`
包支持的平台triplet:`vcpkg help triplet`
安装: `vcpkg install tbb:x64-windows` （默认x86平台)
集成：`vcpkg integrate install`
```

常用包

```
vcpkg install glog brpc gtest gperftools tensorflow-cc
```



## 7.3 Linux dev



##ref

```
https://devblogs.microsoft.com/cppblog/intellisense-for-remote-linux-headers/
https://devblogs.microsoft.com/cppblog/linux-development-with-c-in-visual-studio-2019-wsl-asan-for-linux-separation-of-build-and-debug/
```

### ssh连接失败

```
现象：
工具，选项，跨平台，连接管理器，添加，出现连接到远程系统对话框。
输入信息后，使用sshkey链接linux服务器时，点连接会失败，主机名框变红色，鼠标移到红色的输入矿上提示类似：
连接到xxx时出错，我们已经在此文件C:\Users\admin\AppData\Local\devenv_exe_linux_connection_error_95952_032620.log记录信息。
打开文件看到
System.NotSupportedException: Key 'OPENSSH' is not supported.
   在 liblinux.Ssh.PrivateKeyFile.Open(Stream privateKey, SecureString passPhrase)
   在 liblinux.Ssh.PrivateKeyFile..ctor(String fileName, SecureString passPhrase)
   在 liblinux.RemoteSystemBase.Connect(PrivateKeyConnectionInfo privateKeyConnectionInfo)
   在 liblinux.RemoteSystemBase.Connect()
   在 liblinux.RemoteSystemBase.Connect(ConnectionInfo connectionInfo)
   在 SSHConnectionUI.MainWindow.login_Click(Object sender, RoutedEventArgs e)
   
原因：
https://developercommunity.visualstudio.com/idea/617778/systemnotsupportedexception-key-openssh-is-not-sup.html
当前（2020.3.26，vs2019 16.5.1）自带的openssh插件支持
-----BEGIN RSA PRIVATE KEY-----
格式的私钥，不支持
-----BEGIN OPENSSH PRIVATE KEY-----
格式的私钥。
解决办法:另外生成一份私钥 ssh-keygen -t rsa ，然后将公钥添加到服务器的authrized_keys里面。使用心得rsa格式私钥链接。
```



### 7.3.1 WSL

```
https://devblogs.microsoft.com/cppblog/c-with-visual-studio-2019-and-windows-subsystem-for-linux-wsl/
我的vs版本vs2019 16.4
1. 设置wsl
	apt install g++ gdb make rsync zip
  rsync, zip: 从wsl导出头文件到windows给IntelliSence用，一次性工作
2. CMake项目使用WSL
  1). 新建cmake项目
    文件，新建，项目，cmake，创建，项目名称可选，比如cmake3
  2). 添加wsl-debug配置
    管理下拉列表（一般会显示x64-debug），选择配置管理器，配置菜单点+，选WSL-debug，添加一个wsl配置。有多个wsl的话，在wsl可执行文件中选择。点击保存（文件菜单或工具栏按钮）
    在管理下拉列表最后那个选择wsl-debug，激活当前配置
    系统可能会提示
    “所选配置不适用于此文件。IntelliSence可能不准确。将应用以下配置:x64-Debug"
    “从Cmake手机信息后将刷新C++ IntelliSence信息”
    这是由于还未从wsl提取头文件导致的，会自动处理，稍等即可。此时注意vs输出窗，一般会显示
    2> 已为配置“WSL-Debug”启动 CMake 生成。
    2> 在 /usr/bin/cmake 上找到 cmake 可执行文件。
    后面会有cmake的一些输出，包括检查环境，生成缓存，最后显示cmake生成完毕。
    之后系统会自动刷新。
  3). 调试断点
    在自动生成的cpp中打断点，选择启动项下拉框选择cmake3(cmake3\cmake3)，启动即可。
    输出，生成可以看到生成命令等信息
     cd /mnt/c/dev/cpp/cmake3/out/build/WSL-Debug;/usr/bin/cmake --build /mnt/c/dev/cpp/cmake3/out/build/WSL-Debug --target cmake3 ;
    输出，调试，可以看到断点信息：
     Breakpoint 1, main () at /mnt/c/dev/cpp/cmake3/cmake3/cmake3.cpp:10
     10		cout << "Hello CMake." << endl;
     Loaded '/lib64/ld-linux-x86-64.so.2'. Symbols loaded.
     Loaded '/usr/lib/x86_64-linux-gnu/libstdc++.so.6'. Symbols loaded.
     Loaded '/lib/x86_64-linux-gnu/libc.so.6'. Symbols loaded.
     Loaded '/lib/x86_64-linux-gnu/libm.so.6'. Symbols loaded.
     Loaded '/lib/x86_64-linux-gnu/libgcc_s.so.1'. Symbols loaded.
	Linux控制台可以看到程序的输出
  ** 注意上述涉及的路径，都是是基于wsl路径的信息。说明是在wsl中调试的
3. 基于msbuild-linux的wsl调试
  1)新建，项目，控制台应用（C++ Linux 控制台），名字示例wsl_test3。不需配置linux ssh
  2)解决方案管理器，选择项目wsl_test3(Linux),右键，属性，打开项目属性页
    配置属性，常规，平台工具集，从GCC for Remote Linux，修改为GCC for Windows Subsystem for linux，确定。
  3)打断点，工具栏，GDB调试程序点启动
    可以在输出和Linux控制台窗口看到和2类似的信息
    区别在于使用的msbuild工具链生成的linux程序，所以输出，生成窗口显示使用windows路径
     1>正在链接对象
     1>wsl_test3.vcxproj -> C:\dev\cpp\wsl_test3\wsl_test3\bin\x64\Debug\wsl_test3.out
    但调试和程序输出都在wsl里面。  
```

## 7.4 vs2019无法登录账号

```
社区版需要申请licence，但是无法登录账户申请新的licence，输入账户后下一步卡死。
按照https://blog.csdn.net/cc93691810/article/details/93969472
的方法尝试登出服务后，输入账户后下一步白屏
IdentityServiceAdalCache.cache 文件也未找到。
删除.IdentityService目录也没效果。

解决：
在提示过期的界面上有个账户选项，点击 登录选项 有个黄思勇一下方式添加账户并重新进行身份验证，把 嵌入式web浏览器 改成 系统web浏览器，再次登录会弹出浏览器窗口，登录后返回vs界面会自动刷新账户状态，点检查新的许可证即可更新。
```



# 8 Intelij Idea

## 8.1 Intelij CLion

## 8.2 Intellij IDEA

```
https://plugins.jetbrains.com/plugin/1373-c-c-/reviews#review=28966
可安装C/C++插件，但是需要修改配置支持最新版本。
1. 下载插件zip文件
2. 解压，修改配置，重新打包为zip
3. 从本地安装插件：file, settings, plugins, 齿轮图标，install from disk，指定插件zip文件。
```





# 9. 三方库

## 9.1 内存泄漏检测

#### crtdbg

```
vs自带内存泄露检测。注意文件顺序，否则可能提示不准，或者无法转换行号。
https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019#enable-memory-leak-detection

符号定义及头文件：放在开头
#define _CRTDBG_MAP_ALLOC // 映射行号
#include <stdlib.h>
#include <crtdbg.h>  

// 代码之前
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#endif

// main函数开头
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
```

####  vld

```
visual C++ 内存泄漏检测

https://kinddragon.github.io/vld/
https://github.com/KindDragon/vld/wiki/Using-Visual-Leak-Detector
下载 2.5.1 2017.10.17
https://github.com/KindDragon/vld/releases
https://github.com/KindDragon/vld/releases/download/v2.5.1/vld-2.5.1-setup.exe
https://github-production-release-asset-2e65be.s3.amazonaws.com/566097/583de30e-b359-11e7-99d7-0f1fb50d7654?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAIWNJYAX4CSVEH53A%2F20200327%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20200327T071336Z&X-Amz-Expires=300&X-Amz-Signature=f22e8e5ea9668efa82dc4e8d55247c86009008ffa7a5f5bb7f1f1bb7b2d3d65c&X-Amz-SignedHeaders=host&actor_id=12442931&response-content-disposition=attachment%3B%20filename%3Dvld-2.5.1-setup.exe&response-content-type=application%2Foctet-stream

默认安装路径
C:\Program Files (x86)\Visual Leak Detector

vs2019下的使用：官方支持到vs2015，github最后更新时间是2017.11。但是vs2019也可以勉强使用，但是可能会有问题。
1. 项目添加头文件和lib文件
   解决方案管理器，项目右键，属性，配置选择debug，x64，配置属性，vc++目录
   包含目录添加 C:\Program Files (x86)\Visual Leak Detector\include  // (x86)会被转义为%28x86%29
   库目录添加   C:\Program Files (x86)\Visual Leak Detector\lib\Win64
2. 项目中包含<vld.h>
   需要在预编译头文件(如stdfx.h)之后
   不需要每个文件都添加，只添加一次即可。
   链接的dll如果要调试，dll代码中也要添加。
3. 项目属性，连接器，调试，生成小时信息，修改为：生成经过优化以共享和发布的调试信息 (/DEBUG:FULL)。否则不会显示泄露文件的行号。
   如果出现 应用程序无法正常启动(0xc0150002)， 或者退出时 dbghelp.dll抛出异常等 问题，可以尝试先把C:\Program Files (x86)\Visual Leak Detector\bin\Win64中的所有文件到编译后的执行文件目录，重新编译程序，运行即可。估计是对vs2019支持还不完善。
```

#### gperftools

```
https://github.com/gperftools/gperftools
gperftools (originally Google Performance Tools)
HEAP CHECKER
```

## 9.2 profilling

#### gprof

```
GNU profilling

https://www.gnu.org/software/binutils/
https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.gz (2020-07-24)
https://sourceware.org/binutils/docs-2.35/gprof/index.html
GNU gprof 

https://ftp.gnu.org/old-gnu/Manuals/gprof-2.9.1/html_chapter/gprof_toc.html
The GNU Profiler

https://www.linuxtopia.org/online_books/an_introduction_to_gcc/gccintro_80.html
An Introduction to GCC: 10.2 Using the profiler gprof

```

#### gperftools

```
**** 注意，在docker中使用gperftools需要特殊参数

https://blog.csdn.net/whuzhang16/article/details/93890514
GoogleCPUProfiler在CMake项目中的使用

https://github.com/neeraj9/gperftools-httpd
配套的httpd工具（仿go语言web端）

http://weakyon.com/2018/08/16/summarize-of-gperftools.html
c++性能分析gperftools总结

使用
1. 直接链接 -lprofiler 
2. 运行时链接
LD_PRELOAD="/usr/local/lib/libprofiler.so" CPUPROFILE=./prof5 CPUPROFILESIGNAL=12 ./mt_predict -s
kill -12 `pidof mt_predict`
```

编译源码

```
./autogen.sh
./configure
make -j8
make check // 检查，可选
make install // 安装

问题：
./autogen.sh
./autogen.sh: /usr/bin/autoreconf: /usr/bin/perl: bad interpreter: No such file or directory
yum install perl

Can't locate Data/Dumper.pm in @INC ...
yum install 'perl(Data::Dumper)'

Can't locate Thread/Queue.pm in @INC ...
yum install perl-Thread-Queue

./configure
checking libunwind.h usability... no
configure: WARNING: No frame pointers and no libunwind. Using experimental backtrace capturing via libgcc. Expect crashy cpu profiler.
yum install libunwind-devel


```

make install

```
/bin/sh ./libtool  --tag=CXX   --mode=compile /apps/sylar/bin/g++ -std=gnu++11 -DHAVE_CONFIG_H -I. -I./src  -I./src   -DNO_TCMALLOC_SAMPLES -pthread -DNDEBUG -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -MT src/libtcmalloc_minimal_la-tcmalloc.lo -MD -MP -MF src/.deps/libtcmalloc_minimal_la-tcmalloc.Tpo -c -o src/libtcmalloc_minimal_la-tcmalloc.lo `test -f 'src/tcmalloc.cc' || echo './'`src/tcmalloc.cc
libtool: compile:  /apps/sylar/bin/g++ -std=gnu++11 -DHAVE_CONFIG_H -I. -I./src -I./src -DNO_TCMALLOC_SAMPLES -pthread -DNDEBUG -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -MT src/libtcmalloc_minimal_la-tcmalloc.lo -MD -MP -MF src/.deps/libtcmalloc_minimal_la-tcmalloc.Tpo -c src/tcmalloc.cc  -fPIC -DPIC -o src/.libs/libtcmalloc_minimal_la-tcmalloc.o
libtool: compile:  /apps/sylar/bin/g++ -std=gnu++11 -DHAVE_CONFIG_H -I. -I./src -I./src -DNO_TCMALLOC_SAMPLES -pthread -DNDEBUG -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -MT src/libtcmalloc_minimal_la-tcmalloc.lo -MD -MP -MF src/.deps/libtcmalloc_minimal_la-tcmalloc.Tpo -c src/tcmalloc.cc -o src/libtcmalloc_minimal_la-tcmalloc.o >/dev/null 2>&1
mv -f src/.deps/libtcmalloc_minimal_la-tcmalloc.Tpo src/.deps/libtcmalloc_minimal_la-tcmalloc.Plo
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -DNO_TCMALLOC_SAMPLES -pthread -DNDEBUG -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -version-info 9:5:5 -no-undefined   -o libtcmalloc_minimal.la -rpath /usr/local/lib src/libtcmalloc_minimal_la-tcmalloc.lo    libtcmalloc_minimal_internal.la 
libtool: link: rm -fr  .libs/libtcmalloc_minimal.a .libs/libtcmalloc_minimal.la .libs/libtcmalloc_minimal.lai .libs/libtcmalloc_minimal.so .libs/libtcmalloc_minimal.so.4 .libs/libtcmalloc_minimal.so.4.5.5
libtool: link: /apps/sylar/bin/g++ -std=gnu++11  -fPIC -DPIC -shared -nostdlib /lib/../lib64/crti.o /apps/sylar/lib/gcc/x86_64-pc-linux-gnu/9.1.0/crtbeginS.o  src/.libs/libtcmalloc_minimal_la-tcmalloc.o  -Wl,--whole-archive ./.libs/libtcmalloc_minimal_internal.a -Wl,--no-whole-archive  -Wl,-rpath -Wl,/apps/sylar/lib/../lib64 -Wl,-rpath -Wl,/apps/sylar/lib/../lib64 -L/apps/sylar/lib/gcc/x86_64-pc-linux-gnu/9.1.0 -L/apps/sylar/lib/gcc/x86_64-pc-linux-gnu/9.1.0/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/apps/sylar/lib/gcc/x86_64-pc-linux-gnu/9.1.0/../../.. /apps/sylar/lib/../lib64/libstdc++.so -lm -lc -lgcc_s /apps/sylar/lib/gcc/x86_64-pc-linux-gnu/9.1.0/crtendS.o /lib/../lib64/crtn.o  -pthread -g -O2   -pthread -Wl,-soname -Wl,libtcmalloc_minimal.so.4 -o .libs/libtcmalloc_minimal.so.4.5.5
libtool: link: (cd ".libs" && rm -f "libtcmalloc_minimal.so.4" && ln -s "libtcmalloc_minimal.so.4.5.5" "libtcmalloc_minimal.so.4")
libtool: link: (cd ".libs" && rm -f "libtcmalloc_minimal.so" && ln -s "libtcmalloc_minimal.so.4.5.5" "libtcmalloc_minimal.so")
libtool: link: (cd .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a && ar x "/home/cjx/dev/src/github.com/gperftools/gperftools/./.libs/libtcmalloc_minimal_internal.a")
libtool: link: ar cru .libs/libtcmalloc_minimal.a  src/libtcmalloc_minimal_la-tcmalloc.o  .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/atomicops-internals-x86.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/dynamic_annotations.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-central_freelist.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-common.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-internal_logging.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-malloc_extension.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-malloc_hook.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-memfs_malloc.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-page_heap.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-sampler.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-span.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-stack_trace_table.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-static_vars.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-symbolize.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-system-alloc.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/libtcmalloc_minimal_internal_la-thread_cache.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/logging.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/maybe_threads.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/spinlock.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/spinlock_internal.o .libs/libtcmalloc_minimal.lax/libtcmalloc_minimal_internal.a/sysinfo.o 
libtool: link: ranlib .libs/libtcmalloc_minimal.a
libtool: link: rm -fr .libs/libtcmalloc_minimal.lax
libtool: link: ( cd ".libs" && rm -f "libtcmalloc_minimal.la" && ln -s "../libtcmalloc_minimal.la" "libtcmalloc_minimal.la" )
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread  -static  -o malloc_bench benchmark/malloc_bench-malloc_bench.o librun_benchmark.la libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o malloc_bench benchmark/malloc_bench-malloc_bench.o  ./.libs/librun_benchmark.a ./.libs/libtcmalloc_minimal.a /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/apps/sylar/lib/../lib64 -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o malloc_bench_shared benchmark/malloc_bench_shared-malloc_bench.o librun_benchmark.la libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/malloc_bench_shared benchmark/malloc_bench_shared-malloc_bench.o  ./.libs/librun_benchmark.a ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread  -static  -o binary_trees benchmark/binary_trees-binary_trees.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o binary_trees benchmark/binary_trees-binary_trees.o  ./.libs/libtcmalloc_minimal.a /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/apps/sylar/lib/../lib64 -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o binary_trees_shared benchmark/binary_trees_shared-binary_trees.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/binary_trees_shared benchmark/binary_trees_shared-binary_trees.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o tcmalloc_minimal_unittest src/tests/tcmalloc_minimal_unittest-tcmalloc_unittest.o src/tests/tcmalloc_minimal_unittest-testutil.o libtcmalloc_minimal.la liblogging.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/tcmalloc_minimal_unittest src/tests/tcmalloc_minimal_unittest-tcmalloc_unittest.o src/tests/tcmalloc_minimal_unittest-testutil.o  ./.libs/libtcmalloc_minimal.so ./.libs/liblogging.a /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o tcmalloc_minimal_large_unittest src/tests/tcmalloc_minimal_large_unittest-tcmalloc_large_unittest.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/tcmalloc_minimal_large_unittest src/tests/tcmalloc_minimal_large_unittest-tcmalloc_large_unittest.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o tcmalloc_minimal_large_heap_fragmentation_unittest src/tests/tcmalloc_minimal_large_heap_fragmentation_unittest-large_heap_fragmentation_unittest.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/tcmalloc_minimal_large_heap_fragmentation_unittest src/tests/tcmalloc_minimal_large_heap_fragmentation_unittest-large_heap_fragmentation_unittest.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o system_alloc_unittest src/tests/system_alloc_unittest-system-alloc_unittest.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/system_alloc_unittest src/tests/system_alloc_unittest-system-alloc_unittest.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o packed_cache_test src/tests/packed_cache_test-packed-cache_test.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/packed_cache_test src/tests/packed_cache_test-packed-cache_test.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o frag_unittest src/tests/frag_unittest-frag_unittest.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/frag_unittest src/tests/frag_unittest-frag_unittest.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o markidle_unittest src/tests/markidle_unittest-markidle_unittest.o src/tests/markidle_unittest-testutil.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/markidle_unittest src/tests/markidle_unittest-markidle_unittest.o src/tests/markidle_unittest-testutil.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o current_allocated_bytes_test src/tests/current_allocated_bytes_test-current_allocated_bytes_test.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/current_allocated_bytes_test src/tests/current_allocated_bytes_test-current_allocated_bytes_test.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o malloc_hook_test src/tests/malloc_hook_test-malloc_hook_test.o src/tests/malloc_hook_test-testutil.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/malloc_hook_test src/tests/malloc_hook_test-malloc_hook_test.o src/tests/malloc_hook_test-testutil.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o malloc_extension_test src/tests/malloc_extension_test-malloc_extension_test.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/malloc_extension_test src/tests/malloc_extension_test-malloc_extension_test.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CC   --mode=link /apps/sylar/bin/gcc -pthread  -ansi -g -O2 -pthread   -o malloc_extension_c_test src/tests/malloc_extension_c_test-malloc_extension_c_test.o libtcmalloc_minimal.la -lpthread -lstdc++ -lm 
libtool: link: /apps/sylar/bin/gcc -pthread -ansi -g -O2 -pthread -o .libs/malloc_extension_c_test src/tests/malloc_extension_c_test-malloc_extension_c_test.o  ./.libs/libtcmalloc_minimal.so -lpthread /apps/sylar/lib/../lib64/libstdc++.so -lm -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o memalign_unittest src/tests/memalign_unittest-memalign_unittest.o src/tests/memalign_unittest-testutil.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/memalign_unittest src/tests/memalign_unittest-memalign_unittest.o src/tests/memalign_unittest-testutil.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o page_heap_test src/tests/page_heap_test-page_heap_test.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/page_heap_test src/tests/page_heap_test-page_heap_test.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o pagemap_unittest src/tests/pagemap_unittest-pagemap_unittest.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/pagemap_unittest src/tests/pagemap_unittest-pagemap_unittest.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o realloc_unittest src/tests/realloc_unittest-realloc_unittest.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/realloc_unittest src/tests/realloc_unittest-realloc_unittest.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o stack_trace_table_test src/tests/stack_trace_table_test-stack_trace_table_test.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/stack_trace_table_test src/tests/stack_trace_table_test-stack_trace_table_test.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o thread_dealloc_unittest src/tests/thread_dealloc_unittest-thread_dealloc_unittest.o src/tests/thread_dealloc_unittest-testutil.o libtcmalloc_minimal.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/thread_dealloc_unittest src/tests/thread_dealloc_unittest-thread_dealloc_unittest.o src/tests/thread_dealloc_unittest-testutil.o  ./.libs/libtcmalloc_minimal.so /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
rm -f debugallocation_test.sh
cp -p ./src/tests/debugallocation_test.sh debugallocation_test.sh
rm -f tcmalloc_unittest.sh
cp -p ./src/tests/tcmalloc_unittest.sh tcmalloc_unittest.sh
/bin/sh ./libtool  --tag=CXX   --mode=link /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare  -Wno-unused-result -fsized-deallocation -faligned-new  -DNO_FRAME_POINTER -g -O2 -pthread   -o tcmalloc_both_unittest src/tests/tcmalloc_both_unittest-tcmalloc_unittest.o src/tests/tcmalloc_both_unittest-testutil.o libtcmalloc.la libtcmalloc_minimal.la libprofiler.la liblogging.la -lpthread 
libtool: link: /apps/sylar/bin/g++ -std=gnu++11 -pthread -Wall -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -Wno-unused-result -fsized-deallocation -faligned-new -DNO_FRAME_POINTER -g -O2 -pthread -o .libs/tcmalloc_both_unittest src/tests/tcmalloc_both_unittest-tcmalloc_unittest.o src/tests/tcmalloc_both_unittest-testutil.o  ./.libs/libtcmalloc.so ./.libs/libtcmalloc_minimal.so ./.libs/libprofiler.so -lunwind ./.libs/liblogging.a /apps/sylar/lib/../lib64/libstdc++.so -lm -lpthread -pthread -Wl,-rpath -Wl,/usr/local/lib -Wl,-rpath -Wl,/apps/sylar/lib/../lib64
rm -f sampling_test.sh
cp -p ./src/tests/sampling_test.sh sampling_test.sh
rm -f heap-profiler_unittest.sh
cp -p ./src/tests/heap-profiler_unittest.sh heap-profiler_unittest.sh
rm -f heap-checker_unittest.sh
cp -p ./src/tests/heap-checker_unittest.sh heap-checker_unittest.sh
rm -f heap-checker-death_unittest.sh
cp -p ./src/tests/heap-checker-death_unittest.sh heap-checker-death_unittest.sh
rm -f sampling_debug_test.sh
cp -p ./src/tests/sampling_test.sh sampling_debug_test.sh
rm -f heap-profiler_debug_unittest.sh
cp -p ./src/tests/heap-profiler_unittest.sh heap-profiler_debug_unittest.sh
rm -f heap-checker_debug_unittest.sh
cp -p ./src/tests/heap-checker_unittest.sh heap-checker_debug_unittest.sh
rm -f profiler_unittest.sh
cp -p ./src/tests/profiler_unittest.sh profiler_unittest.sh
for la in libtcmalloc_minimal.la libtcmalloc_minimal_debug.la libtcmalloc.la libtcmalloc_debug.la libtcmalloc_and_profiler.la; do lib=".libs/`basename $la .la`.a"; [ ! -f "$lib" ] || : "$lib"; done
make[1]: Entering directory `/home/cjx/dev/src/github.com/gperftools/gperftools'
 /usr/bin/mkdir -p '/usr/local/lib'
 /bin/sh ./libtool   --mode=install /usr/bin/install -c   libtcmalloc_minimal.la libtcmalloc_minimal_debug.la libtcmalloc.la libtcmalloc_debug.la libprofiler.la libtcmalloc_and_profiler.la '/usr/local/lib'
libtool: install: /usr/bin/install -c .libs/libtcmalloc_minimal.so.4.5.5 /usr/local/lib/libtcmalloc_minimal.so.4.5.5
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_minimal.so.4.5.5 libtcmalloc_minimal.so.4 || { rm -f libtcmalloc_minimal.so.4 && ln -s libtcmalloc_minimal.so.4.5.5 libtcmalloc_minimal.so.4; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_minimal.so.4.5.5 libtcmalloc_minimal.so || { rm -f libtcmalloc_minimal.so && ln -s libtcmalloc_minimal.so.4.5.5 libtcmalloc_minimal.so; }; })
libtool: install: /usr/bin/install -c .libs/libtcmalloc_minimal.lai /usr/local/lib/libtcmalloc_minimal.la
libtool: install: /usr/bin/install -c .libs/libtcmalloc_minimal_debug.so.4.5.5 /usr/local/lib/libtcmalloc_minimal_debug.so.4.5.5
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_minimal_debug.so.4.5.5 libtcmalloc_minimal_debug.so.4 || { rm -f libtcmalloc_minimal_debug.so.4 && ln -s libtcmalloc_minimal_debug.so.4.5.5 libtcmalloc_minimal_debug.so.4; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_minimal_debug.so.4.5.5 libtcmalloc_minimal_debug.so || { rm -f libtcmalloc_minimal_debug.so && ln -s libtcmalloc_minimal_debug.so.4.5.5 libtcmalloc_minimal_debug.so; }; })
libtool: install: /usr/bin/install -c .libs/libtcmalloc_minimal_debug.lai /usr/local/lib/libtcmalloc_minimal_debug.la
libtool: install: /usr/bin/install -c .libs/libtcmalloc.so.4.5.5 /usr/local/lib/libtcmalloc.so.4.5.5
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc.so.4.5.5 libtcmalloc.so.4 || { rm -f libtcmalloc.so.4 && ln -s libtcmalloc.so.4.5.5 libtcmalloc.so.4; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc.so.4.5.5 libtcmalloc.so || { rm -f libtcmalloc.so && ln -s libtcmalloc.so.4.5.5 libtcmalloc.so; }; })
libtool: install: /usr/bin/install -c .libs/libtcmalloc.lai /usr/local/lib/libtcmalloc.la
libtool: install: /usr/bin/install -c .libs/libtcmalloc_debug.so.4.5.5 /usr/local/lib/libtcmalloc_debug.so.4.5.5
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_debug.so.4.5.5 libtcmalloc_debug.so.4 || { rm -f libtcmalloc_debug.so.4 && ln -s libtcmalloc_debug.so.4.5.5 libtcmalloc_debug.so.4; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_debug.so.4.5.5 libtcmalloc_debug.so || { rm -f libtcmalloc_debug.so && ln -s libtcmalloc_debug.so.4.5.5 libtcmalloc_debug.so; }; })
libtool: install: /usr/bin/install -c .libs/libtcmalloc_debug.lai /usr/local/lib/libtcmalloc_debug.la
libtool: install: /usr/bin/install -c .libs/libprofiler.so.0.5.0 /usr/local/lib/libprofiler.so.0.5.0
libtool: install: (cd /usr/local/lib && { ln -s -f libprofiler.so.0.5.0 libprofiler.so.0 || { rm -f libprofiler.so.0 && ln -s libprofiler.so.0.5.0 libprofiler.so.0; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libprofiler.so.0.5.0 libprofiler.so || { rm -f libprofiler.so && ln -s libprofiler.so.0.5.0 libprofiler.so; }; })
libtool: install: /usr/bin/install -c .libs/libprofiler.lai /usr/local/lib/libprofiler.la
libtool: install: /usr/bin/install -c .libs/libtcmalloc_and_profiler.so.4.6.0 /usr/local/lib/libtcmalloc_and_profiler.so.4.6.0
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_and_profiler.so.4.6.0 libtcmalloc_and_profiler.so.4 || { rm -f libtcmalloc_and_profiler.so.4 && ln -s libtcmalloc_and_profiler.so.4.6.0 libtcmalloc_and_profiler.so.4; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libtcmalloc_and_profiler.so.4.6.0 libtcmalloc_and_profiler.so || { rm -f libtcmalloc_and_profiler.so && ln -s libtcmalloc_and_profiler.so.4.6.0 libtcmalloc_and_profiler.so; }; })
libtool: install: /usr/bin/install -c .libs/libtcmalloc_and_profiler.lai /usr/local/lib/libtcmalloc_and_profiler.la
libtool: install: /usr/bin/install -c .libs/libtcmalloc_minimal.a /usr/local/lib/libtcmalloc_minimal.a
libtool: install: chmod 644 /usr/local/lib/libtcmalloc_minimal.a
libtool: install: ranlib /usr/local/lib/libtcmalloc_minimal.a
libtool: install: /usr/bin/install -c .libs/libtcmalloc_minimal_debug.a /usr/local/lib/libtcmalloc_minimal_debug.a
libtool: install: chmod 644 /usr/local/lib/libtcmalloc_minimal_debug.a
libtool: install: ranlib /usr/local/lib/libtcmalloc_minimal_debug.a
libtool: install: /usr/bin/install -c .libs/libtcmalloc.a /usr/local/lib/libtcmalloc.a
libtool: install: chmod 644 /usr/local/lib/libtcmalloc.a
libtool: install: ranlib /usr/local/lib/libtcmalloc.a
libtool: install: /usr/bin/install -c .libs/libtcmalloc_debug.a /usr/local/lib/libtcmalloc_debug.a
libtool: install: chmod 644 /usr/local/lib/libtcmalloc_debug.a
libtool: install: ranlib /usr/local/lib/libtcmalloc_debug.a
libtool: install: /usr/bin/install -c .libs/libprofiler.a /usr/local/lib/libprofiler.a
libtool: install: chmod 644 /usr/local/lib/libprofiler.a
libtool: install: ranlib /usr/local/lib/libprofiler.a
libtool: install: /usr/bin/install -c .libs/libtcmalloc_and_profiler.a /usr/local/lib/libtcmalloc_and_profiler.a
libtool: install: chmod 644 /usr/local/lib/libtcmalloc_and_profiler.a
libtool: install: ranlib /usr/local/lib/libtcmalloc_and_profiler.a
libtool: finish: PATH="/home/cjx/bin/go/bin:/apps/sylar/bin:/apps/sylar/sbin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/cjx/script:/home/cjx/script/bin:/usr/local/anaconda3/bin:/sbin" ldconfig -n /usr/local/lib
----------------------------------------------------------------------
Libraries have been installed in:
   /usr/local/lib

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
 /usr/bin/mkdir -p '/usr/local/bin'
 /usr/bin/install -c pprof-symbolize src/pprof '/usr/local/bin'
for la in libtcmalloc_minimal.la libtcmalloc_minimal_debug.la libtcmalloc.la libtcmalloc_debug.la libtcmalloc_and_profiler.la; do lib=".libs/`basename $la .la`.a"; [ ! -f "$lib" ] || : "$lib"; done
 /usr/bin/mkdir -p '/usr/local/share/doc/gperftools'
 /usr/bin/install -c -m 644 AUTHORS COPYING ChangeLog INSTALL NEWS README README_windows.txt TODO ChangeLog.old docs/index.html docs/designstyle.css docs/pprof_remote_servers.html docs/tcmalloc.html docs/overview.gif docs/pageheap.gif docs/spanmap.gif docs/threadheap.gif docs/t-test1.times.txt docs/tcmalloc-opspercpusec.vs.threads.1024.bytes.png docs/tcmalloc-opspercpusec.vs.threads.128.bytes.png docs/tcmalloc-opspercpusec.vs.threads.131072.bytes.png docs/tcmalloc-opspercpusec.vs.threads.16384.bytes.png docs/tcmalloc-opspercpusec.vs.threads.2048.bytes.png docs/tcmalloc-opspercpusec.vs.threads.256.bytes.png docs/tcmalloc-opspercpusec.vs.threads.32768.bytes.png docs/tcmalloc-opspercpusec.vs.threads.4096.bytes.png docs/tcmalloc-opspercpusec.vs.threads.512.bytes.png docs/tcmalloc-opspercpusec.vs.threads.64.bytes.png docs/tcmalloc-opspercpusec.vs.threads.65536.bytes.png docs/tcmalloc-opspercpusec.vs.threads.8192.bytes.png docs/tcmalloc-opspersec.vs.size.1.threads.png docs/tcmalloc-opspersec.vs.size.12.threads.png docs/tcmalloc-opspersec.vs.size.16.threads.png docs/tcmalloc-opspersec.vs.size.2.threads.png docs/tcmalloc-opspersec.vs.size.20.threads.png docs/tcmalloc-opspersec.vs.size.3.threads.png docs/tcmalloc-opspersec.vs.size.4.threads.png docs/tcmalloc-opspersec.vs.size.5.threads.png docs/tcmalloc-opspersec.vs.size.8.threads.png docs/overview.dot '/usr/local/share/doc/gperftools'
 /usr/bin/install -c -m 644 docs/pageheap.dot docs/spanmap.dot docs/threadheap.dot docs/heapprofile.html docs/heap-example1.png docs/heap_checker.html docs/cpuprofile.html docs/cpuprofile-fileformat.html docs/pprof-test-big.gif docs/pprof-test.gif docs/pprof-vsnprintf-big.gif docs/pprof-vsnprintf.gif '/usr/local/share/doc/gperftools'
 /usr/bin/mkdir -p '/usr/local/include/google'
 /usr/bin/install -c -m 644 src/google/heap-checker.h src/google/heap-profiler.h src/google/malloc_extension.h src/google/malloc_extension_c.h src/google/malloc_hook.h src/google/malloc_hook_c.h src/google/profiler.h src/google/stacktrace.h src/google/tcmalloc.h '/usr/local/include/google'
 /usr/bin/mkdir -p '/usr/local/share/man/man1'
 /usr/bin/install -c -m 644 docs/pprof.1 '/usr/local/share/man/man1'
 /usr/bin/mkdir -p '/usr/local/include/gperftools'
 /usr/bin/install -c -m 644 src/gperftools/tcmalloc.h '/usr/local/include/gperftools'
 /usr/bin/mkdir -p '/usr/local/include/gperftools'
 /usr/bin/install -c -m 644 src/gperftools/stacktrace.h src/gperftools/malloc_hook.h src/gperftools/malloc_hook_c.h src/gperftools/malloc_extension.h src/gperftools/malloc_extension_c.h src/gperftools/nallocx.h src/gperftools/heap-profiler.h src/gperftools/heap-checker.h src/gperftools/profiler.h '/usr/local/include/gperftools'
 /usr/bin/mkdir -p '/usr/local/lib/pkgconfig'
 /usr/bin/install -c -m 644 libtcmalloc.pc libtcmalloc_minimal.pc libtcmalloc_debug.pc libtcmalloc_minimal_debug.pc libprofiler.pc '/usr/local/lib/pkgconfig'
make[1]: Leaving directory `/home/cjx/dev/src/github.com/gperftools/gperftools'
```

## 9.3 faiss

```
facebook的向量搜索库

https://zhuanlan.zhihu.com/p/40236865
https://github.com/facebookresearch/faiss/wiki/Getting-started
https://github.com/facebookresearch/faiss/wiki/Faster-search
https://github.com/facebookresearch/faiss/wiki/Lower-memory-footprint

##############
conda安装python库
下载miniconda3，然后按照教程安装即可。


##############
编译源码
1. 安装依赖库
apt install openblas-dev liblapack-dev
yum install openblas-devel lapack64-devel
可以通过apt search blas/lapack或者yum search blas/lapack查找具体包名

也可以源码编译openblas https://juejin.im/post/5bed7cebe51d454e5b5f2c23
git clone https://github.com/xianyi/OpenBLAS.git
cd OpenBLAS
#yum install gcc-gfortran （unbuntu版 执行 sudo apt-get install gfortran）
make FC=gfortran
make install
ln -s /opt/OpenBLAS/lib/libopenblas.so  /usr/lib/libopenblas.so

2. 配置
./configure --without-cuda
3. 检查
make misc/test_blas
./misc/test_blas
会提示lapack具体使用的int字长。如
Intentional Lapack error (appears only for 64-bit INTEGER):
info=0000064b00000000
Lapack uses 32-bit integers
根据https://github.com/facebookresearch/faiss/issues/101这个只是个提示。
如果make过程提示找不到openblas库，
export  export LD_LIBRARY_PATH=<open-blas-lib-path>

4. 编译
make

5. 安装
make install
cp libfaiss.a libfaiss.so /usr/local/lib
tar cf - MatrixStats.h IndexBinaryHNSW.h Index2Layer.h IndexPreTransform.h IndexIVFFlat.h MetaIndexes.h IndexScalarQuantizer.h IndexPQ.h Clustering.h IVFlib.h IndexBinary.h Index.h IndexHNSW.h IndexIVFPQR.h IndexFlat.h IndexBinaryHash.h DirectMap.h IndexBinaryFromFloat.h IndexBinaryIVF.h IndexIVFPQ.h OnDiskInvertedLists.h VectorTransform.h IndexReplicas.h index_io.h IndexLattice.h index_factory.h InvertedLists.h IndexBinaryFlat.h IndexLSH.h IndexIVF.h IndexShards.h MetricType.h clone_index.h IndexIVFSpectralHash.h AutoTune.h impl/AuxIndexStructures.h impl/FaissAssert.h impl/PolysemousTraining.h impl/io.h impl/HNSW.h impl/ThreadedIndex-inl.h impl/ScalarQuantizer.h impl/FaissException.h impl/lattice_Zn.h impl/ProductQuantizer.h impl/ProductQuantizer-inl.h impl/ThreadedIndex.h utils/WorkerThread.h utils/extra_distances.h utils/distances.h utils/hamming.h utils/hamming-inl.h utils/random.h utils/utils.h utils/Heap.h | tar xf - -C /usr/local/include/faiss/
可以根据需要把头文件复制到自己需要的目录

6. demo
make -C demos demo_ivfpq_indexing
cd demos
./demo_ivfpq_indexing
提示找不到libfaiss.so时export LD_LIBRARY_PATH=<libfaiss.so-path>
```

## 9.4 brpc

```
https://github.com/apache/incubator-brpc/blob/master/docs/cn/getting_started.md

1. 安装依赖
sudo apt-get install -y git g++ make libssl-dev libgflags-dev libprotobuf-dev libprotoc-dev protobuf-compiler libleveldb-dev  libsnappy-dev libgoogle-perftools-dev
sudo apt-get install -y cmake libgtest-dev && cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libgtest* /usr/lib/ && cd -
2. 编译
sh config_brpc.sh --headers=/usr/include --libs=/usr/lib
make -j 64 // 并发编译
3. 运行demo
cd example/echo_c++
make
./echo_server &
./echo_client
```

## 9.5 clang-format

```
llvm-project(clang-format)

git clone -b llvmorg-10.0.0 --single-branch --depth 1 https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build (in-tree build is not supported)
cd build
cmake -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" ../llvm
make -j64
make install # 会安装很多东西

** gcc8编译时需要camke 3.4.3 
centos需要升级camke
cmake --verson                # 查看当前版本
yum remove cmake              # 移除老版本 
yum install https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm # 安装epel源
yum install cmake3            # 安装cmake3
ln -s /bin/cmake3 /bin/cmake  # 软连接
```

## 9.6 abseil

```
Google Abseil 
https://abseil.io/
https://github.com/abseil/abseil-cpp
https://zhuanlan.zhihu.com/p/29940200

基础类库，作为C++ STL的补充
Abseil 简要组成部分如下：
base Abseil Fundamentals ：包含初始化代码和其它部分依赖的代码。除了 C++ 标准库外不依赖外部代码
algorithm ：C++ <algorithm> 库的增强
container ：STL 风格容器
debugging ：内存泄露检查
memory ：智能指针和内存管理
meta ：用 C++11 兼容代码支持 C++14 和 C++17 版本的 <type_traits> 库
numeric ：支持 C++11 兼容的 128 位整数
strings ：string 相关函数增强
synchronization ：同步原语和抽象支持
time ：时间方面的计算
types ：非容器类型的工具类型
```

## 9.7 数学表达式求值

```
https://github.com/ArashPartow/math-parser-benchmark-project
ExprTk：仅头文件，支持多类型，功能完善，支持常见计算、逻辑判断、流程控制、自定义表达式等。模板实现，编译耗时较长。
TinyExpr：简单小巧，仅支持普通数学表达式和自定义函数，效率尚可，编译较快。
```







# 10. 操作示例

## 10.1 linux编译

多个源文件->编译为目标文件->链接生成可执行文件

```
g++单独编译多个源文件
main.cpp  // 自带Calc签名，调用Calc函数
module/calc.cpp  //int Calc(int x, int y); 

1. 编译目标文件
g++ -c module/calc.cpp // 生成calc.o
g++ -c main.cpp // 生成main.o
2. 链接生成可执行文件
g++ main.o module/calc.o // 生成a.out

可能的问题：
1. 链接时提示undefined reference.
   **排查方法**：用nm分别查看调用方和被调用方的目标文件，看签名是否一致。
   main.cpp:(.text+0x21): undefined reference to `Calc(int, int)'
   原因：main调用的函数和calc生成的函数签名不一致。
   注意：c和c++编译出来的同一个函数签名不同。g++/gcc编译时，由函数签名前的 extern "C" 前缀决定
   nm xxx.o可以显示导出函数签名
   编译结果calc：
   extern "C" int Calc(int x, int y);
		0000000000000000 T Calc
   int Calc(int x, int y);
		0000000000000000 T _Z4Calcii
		
   编译结果main:
   extern "C" int Calc(int x, int y);
                 		 U Calc
		0000000000000000 T main  
   int Calc(int x, int y);
		0000000000000000 T main 
                          U _Z4Calcii
   
   可以看到，extern "C"修饰的函数，链接符号和本地符号都相同。T应该是本地函数，U是指需要链接的符号。

2. gcc编译的.o，可以通过g++链接
   nm看到的函数签名，gcc和g++相同
   readelf -s *.so也可以看到导出符号
```

## 10.2 跨平台宏定义

```
#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif

// vs debug
_DEBUG
```

## 10.3 标准宏定义

```
https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
由标准定义，因此应该所有复合标准的额编译器都支持
__FILE__
__LINE__
__DATE__
__TIME__
__func__
__STDC__ // to 1 if comforms ISO Standard C
__STDC_VERSION__
__cplusplus // to 1 if C++
```

# 11. 笔记

## 11.1 性能

### 耗时采样

```
**** clock_gettime大约比chrono快4倍(63ns -> 239nm).但是纳秒级耗时一般不用在意。
**** c
static constexpr uint64_t kSecondsToNanos = 1000ULL * 1000ULL * 1000ULL;
inline uint64_t nanoTime() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (static_cast<uint64_t>(ts.tv_sec) * kSecondsToNanos +
          static_cast<uint64_t>(ts.tv_nsec));
}

**** c++

    auto tp = chrono::high_resolution_clock::now();
    diff = chrono::duration_cast<chrono::nanoseconds>(
            chrono::high_resolution_clock::now() - tp)
            .count();

**** 测试代码 loop=1e6
  cout << "test: " << endl;
  int64_t start = chrono::duration_cast<chrono::nanoseconds>(
                      chrono::high_resolution_clock::now().time_since_epoch())
                      .count();
  for (int i = 0; i < loop; i++) {
    auto tp = chrono::high_resolution_clock::now();
    x = chrono::duration_cast<chrono::nanoseconds>(
            chrono::high_resolution_clock::now() - tp)
            .count();
  }
  auto end = chrono::duration_cast<chrono::nanoseconds>(
                 chrono::high_resolution_clock::now().time_since_epoch())
                 .count();
  auto diff = end - start;
  cout << "test x " << x << " diff " << diff << " avg " << diff / loop << endl;

  cout << "test2: " << endl;
  start = chrono::duration_cast<chrono::nanoseconds>(
              chrono::high_resolution_clock::now().time_since_epoch())
              .count();
  for (int i = 0; i < loop; i++) {
    auto tp = nanoTime();
    x = nanoTime() - tp;
  }
  end = chrono::duration_cast<chrono::nanoseconds>(
            chrono::high_resolution_clock::now().time_since_epoch())
            .count();
  diff = end - start;
  cout << "test2 x " << x << " diff " << diff << " avg " << diff / loop << endl;

**** 测试结果
test: 
test x 120 diff 239566447 avg 239
test2: 
test2 x 30 diff 63204790 avg 63

**** 相关解释
https://gcc.gnu.org/legacy-ml/gcc-help/2014-01/msg00093.ht
Re: Why does std::chrono now() uses slow syscall?
因为多线程可能导致的性能问题，glibc 2.17以后的版本才会使用clock_gettime
```

