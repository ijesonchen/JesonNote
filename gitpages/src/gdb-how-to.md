---
title: GDB NOTE
date: 2020-04-25 00:00 :00
tags: [gdb, note]
---

**GDB NOTE**

how to use gdb in Linux

<!--more-->

[TOC]

# 调试core dump

core dump生成

```
程序crash时，会生成core dump文件。win10下wsl无法生成core dump（已知bug）,wsl2据说可以，但是需要修改配置。
Linux系统，需要配置ulimit
ulimit -a显示配置
ulimit -c unlimited // core dump不限制长度。为0时禁止生成core dump
一般core dump文件在当前目录，core.<pid>。可以配置生成规则。
```

## 1. 简单示例

核心：gcc -g添加调试信息，gdb使用file加载二进制符号

```
main.c:
int main(void){
	char* p = 0;
	*p = 0;
	return 0;
}

gcc main.c -g // 编译main.c，-g表示带debug信息，默认生成 a.out
./app // 执行
13719 segmentation fault (core dumped)  ./a.out   // 13719是进程id
调试：
gdb -c core.13719
Core was generated by `./a.out'.
Program terminated with signal 11, Segmentation fault.
#0  0x00000000004004ad in ?? ()
(gdb) bt   // back trace
#0  0x00000000004004ad in ?? ()
#1  0x00000000004004c0 in ?? ()
#2  0x00007fc558b1b2ab in ?? ()
#3  0x0000000000000000 in ?? ()
(gdb) file a.out  // 加载符号
Reading symbols from /home/cjx/dev/src/git.qutoutiao.net/data_process/feature_process/a.out...done.
(gdb) bt  
#0  0x00000000004004ad in main () at main.c:3  // 第三行
(gdb) l  // list 
1	int main(void){
2	char* p = 0;
3	*p = 0;
4	return 0;
5	}
(gdb) quit
```

## 2. bazel添加debug信息

```
https://docs.bazel.build/versions/master/user-manual.html
需要添加 -c dbg即可。不需要修改bazel配置文件
bazel build -c dbg <proj>
其他和上述示例相同
```



# title 2







