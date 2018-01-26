---
title: how to vcpkg offline
date: 2018-01-10 00:00:00
tags: [how-to, vcpkg, 离线]
---

**如何离线安装vcpkg**

离线内网安装vcpkg的方法：开发网络无法访问internet，可以离线复制文件；另外有一台可以连接internet的开发机，安装好vcpkg后，把相应文件导入到内网即可正常使用。

测试环境：

windows: 10 pro / server 2008 r2

vcpkg: 0.0.91-2017-10-09-d3c45b0668e00703facc86aee998cbfb40fde2ff

visual studio: 2015 update 3

<!--more-->


# 离线安装vcpkg步骤

1. 在线安装vcpkg

```
使用一台可以正常连接internet的电脑，从github： https://github.com/Microsoft/vcpkg下载代码，并编译生成vcpkg
命令行：
	vcpkg search  // 检索已有包
	vcpkg install [pkgname[:pkgarc]] // 安装包，如vcpkg install zlib:x64-windows
```

2. 将相应文件复制到内网机器

```
[installed]	// 包文件
[scripts]	// 集成vs要用到的脚本
[triplets]	// cmake参数。不确定是不是必须
.vcpkg-root	// 主目录标识文件
vcpkg.exe	// 主程序
```

3. 集成到visual studio


```
命令行：
	vcpkg integrate install
```

