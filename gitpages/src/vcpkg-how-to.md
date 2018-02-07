---
title: vcpkg how to
date: 2018-01-10 00:00:00
tags: [how-to, vcpkg, 离线]
---

**vcpkg使用说明**

vcpkg使用说明

[TOC]

<!--more-->

# 1. vcpkg安装

## 1.1. 准备工作

```
Windows 10, 8.1, or 7
Visual Studio 2017 or Visual Studio 2015 Update 3
Git
Optional: CMake 3.10.2
```

## 1.2. 安装

```
// 从github： https://github.com/Microsoft/vcpkg下载代码
git clone https://github.com/Microsoft/vcpkg.git
// 编译生成 vcpkg.exe
cd $vcpkg
bootstrap-vcpkg.bat

```

## 1.3. 使用

	vcpkg search  // 检索已有包
	vcpkg install [pkgname[:pkgarc]] // 安装包，如vcpkg install zlib:x64-windows
## 1.4. 常见问题

### 1.4.1. 无法获取依赖

```
The following packages will be built and installed:
    gtest[core]:x64-windows
Starting package 1/1: gtest:x64-windows
Building package gtest[core]:x64-windows...
A suitable version of git was not found (required v2.15.0). Downloading portable git v2.15.0...
Fetching git version 2.15.0 (No sufficient installed version was found)
Could not run:
    'C:/bin/vcpkg/scripts/fetchDependency.ps1'
Error message was:
    Downloading git...
Ã¯Â»Â¿Exception calling "DownloadFile" with "2" argument(s): "Unable to connect to the remote server"
At C:\bin\vcpkg\scripts\VcpkgPowershellUtils.ps1:116 char:21
+     $wc.DownloadFile <<<< ($url, $downloadPartPath)
    + CategoryInfo          : NotSpecified: (:) [], MethodInvocationException
    + FullyQualifiedErrorId : DotNetMethodException

Move-Item : Cannot find path 'C:\bin\vcpkg\downloads\MinGit-2.15.0-32-bit.zip.part' because it does not exist.
At C:\bin\vcpkg\scripts\VcpkgPowershellUtils.ps1:117 char:14
+     Move-Item <<<<  -Path $downloadPartPath -Destination $downloadPath
    + CategoryInfo          : ObjectNotFound: (C:\bin\vcpkg\do...32-bit.zip.part:String) [Move-Item], ItemNotFoundExce
   ption
    + FullyQualifiedErrorId : PathNotFound,Microsoft.PowerShell.Commands.MoveItemCommand

Downloading git has completed successfully.
Exception calling "ReadAllBytes" with "1" argument(s): "Could not find file 'C:\bin\vcpkg\downloads\MinGit-2.15.0-32-bit.zip'."
```

重点部分日志：

```
Exception calling "DownloadFile"
Exception calling "ReadAllBytes"
C:\bin\vcpkg\downloads\MinGit-2.15.0-32-bit.zip
```

说明：

```
1. 下载依赖 git version 2.15.0 失败
2. 下载失败后会尝试**直接**处理下载后的文件（即使失败了也会继续）
3. 文件名MinGit-2.15.0-32-bit.zip
```

解决：

```
一般是由于网络问题导致。大部分是由于相应依赖对应的网站被墙（一般是亚马逊云 s3.amazonaws.com 的资源）
解决方法是从其他途径下载相应文件，然后放入到 $vcpkg\download 目录中。注意文件名一定要一样。版本是否需要一致未测试
```

常见下载失败的依赖

```
MinGit-2.15.0-32-bit.zip (gtest)
ninja-win.zip (gtest)
```

### 1.4.2 zlib:x64-windows安装失败

```
The following packages will be built and installed:
    zlib[core]:x64-windows
Starting package 1/1: zlib:x64-windows
Building package zlib[core]:x64-windows...
-- CURRENT_INSTALLED_DIR=C:/bin/vcpkg/installed/x64-windows
-- DOWNLOADS=C:/bin/vcpkg/downloads
-- CURRENT_PACKAGES_DIR=C:/bin/vcpkg/packages/zlib_x64-windows
-- CURRENT_BUILDTREES_DIR=C:/bin/vcpkg/buildtrees/zlib
-- CURRENT_PORT_DIR=C:/bin/vcpkg/ports/zlib/.
-- Using cached C:/bin/vcpkg/downloads/zlib1211.tar.gz
-- Testing integrity of cached file...
-- Testing integrity of cached file... OK
-- Extracting source C:/bin/vcpkg/downloads/zlib1211.tar.gz
-- Extracting done
-- Applying patch C:/bin/vcpkg/ports/zlib/cmake_dont_build_more_than_needed.patch
-- Applying patch C:/bin/vcpkg/ports/zlib/cmake_dont_build_more_than_needed.patch done
-- Configuring x64-windows-rel
-- Configuring x64-windows-rel done
-- Configuring x64-windows-dbg
-- Configuring x64-windows-dbg done
-- Build x64-windows-rel
********************************************
CMake Error at scripts/cmake/vcpkg_build_cmake.cmake:129 (message):
    Command failed: C:/bin/vcpkg/downloads/cmake-3.10.2-win32-x86/bin/cmake.exe;--build;.;--config;Release;--target;install;--;-v;-j1
    Working Directory: C:/bin/vcpkg/buildtrees/zlib/x64-windows-rel
    See logs for more information:
      C:\bin\vcpkg\buildtrees\zlib\install-x64-windows-rel-out.log

Call Stack (most recent call first):
  scripts/cmake/vcpkg_install_cmake.cmake:24 (vcpkg_build_cmake)
  ports/zlib/portfile.cmake:26 (vcpkg_install_cmake)
  scripts/ports.cmake:72 (include)


Error: Building package zlib:x64-windows failed with: BUILD_FAILED
Please ensure you're using the latest portfiles with `.\vcpkg update`, then
submit an issue at https://github.com/Microsoft/vcpkg/issues including:
  Package: zlib:x64-windows
  Vcpkg version: 0.0.103-2018-01-27-340ff984cba132f71e4fabbc78c247ad4e4de292

Additionally, attach any relevant sections from the log files above.

**********************************
log
cl: 命令行 error D8000 
```

解决：

```
https://github.com/Microsoft/vcpkg/issues/2105
https://github.com/Microsoft/vcpkg/issues/1939
安装vs15英文语言包后解决。
```

# 如何离线安装vcpkg

离线内网安装vcpkg的方法：开发网络无法访问internet，可以离线复制文件；另外有一台可以连接internet的开发机，安装好vcpkg后，把相应文件导入到内网即可正常使用。

测试环境：

windows: 10 pro / server 2008 r2

vcpkg: 0.0.91-2017-10-09-d3c45b0668e00703facc86aee998cbfb40fde2ff

visual studio: 2015 update 3

1. 在线安装vcpkg



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

