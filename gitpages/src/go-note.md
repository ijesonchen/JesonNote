---
title: Note for Go Language
date: 2018-05-14 00:00:00
tags: [note, Go, gopl, GoInAction]
---

# Note for Go Language

Note for go language.

<!--more-->

[TOC]

## 1. vscode下go开发环境配置

os: win10 pro build 1709

1. **git**: https://git-scm.com Git-2.17.0-64-bit.exe 

2. **tortoise git**: 可选 https://tortoisegit.org/download TortoiseGit-2.6.0.0-64bit.msi 

3. **go lang**: https://golang.org/dl go1.10.2.windows-amd64.msi 

4. **vscode**: https://code.visualstudio.com 1.23.1/20180510

5. **vscode插件**: Go for Visual Studio Code 插件

6. **配置vscode参数**：vscode使用json配置文件。按照如下顺序确定配置项内容。

```
   工作区设置（工作目录下.vscode\settings.json)
   用户设置 (c:\users\[用户名]\AppData\Roaming\Code\User\settings.json)
   默认配置
```

​	修改方法：文件，首选项，会打开配置设置，右侧用户设置，直接使用json覆盖默认设置对应字段即可:

```
{
	// 设置终端为cmd。默认是powershell
    "terminal.integrated.shell.windows": "C:\\WINDOWS\\System32\\cmd.exe",
	// http代理。防止更新插件出错。网络好可忽略
    "http.proxy": "http://127.0.0.1:1080",
    // go安装目录根目录。
    "go.goroot": "C:/bin/Go",
    // go工作目录。一些工具/插件安装位置。不配置的话默认是 c:\users\[用户名]\go
    "go.gopath": "C:/data/gopath",
    // 保存时自动引入未引用的包
    "go.autocompleteUnimportedPackages" : true,
    // 保存时不自动格式化（保存时不会自动移除未引用的package），不建议
     "[go]": {
        "editor.formatOnSave": false
      }
}   
```

7. 安装常用go插件

   新建并打开文件夹(go项目文件夹)，新建xx.go文件。随便写一些代码，保存，系统会提示是安装某些插件。点install all即可自动安装。插件包括：

```
gocode
gopkgs
go-outline
go-symbols
guru
gorename
godef
goreturns
golint	
dlv

gomodifytags
goplay
impl
fillstruct
godoc
gotests
```
## 2. 插件介绍

### 2.1 源码格式化与检查

golint, govet, gofmt

- [ ] [Going the extra mile: golint and go vet](https://splice.com/blog/going-extra-mile-golint-go-vet)
- [ ] [If you code in Go, don't forget to vet](https://www.spreadsheetdb.io/blog/2017/03/if-you-code-in-go-dont-forget-to-vet)    [如果你用Go，不要忘了vet](https://studygolang.com/articles/9619)

https://en.wikipedia.org/wiki/Lint_%28software%29

**gofmt**：格式化工具

**golint**: 代码检查工具，侧重代码风格，检查不规范用法。A linter or lint refers to tools that analyze source code to flag programming errors, bugs, stylistic errors, and suspicious constructs.

**govet**(gotoolvet): 代码错误检查，bug或可疑的构造

# 3. 读书笔记: Go in Action（Go语言实战）

https://github.com/goinaction/code

可参考[飞雪无情的博客](http://www.flysnow.org/categories/Golang/) [Go语言实战笔记（一）| Go包管理](http://www.flysnow.org/2017/03/04/go-in-action-go-package.html) 等

## 3.1 介绍 

- 现代、快速，带有强大标准库
- 内置并发支持goroutine
- 使用接口作为代码复用基础模块

## 3.2 快速开始

# 4. 读书笔记：The GO Programming Language



## 4.10 包和go工具

- 包管理机制: 加速编译。导入显示列出；无循环依赖；目标文件包含所有依赖的导出。
- 导入路径: 建议使用网络域名开始
- 包名：一般是导入路径最后一段。例外：main包；xx_test.go外部测试包；版本号后缀不计入包名。
- 导入声明：每个import可以导入单个或多个；可以加空行分组；建议字母排序；重命名导入；空白导入；循环导入会报错；

```
import "pkgnam1"
import (
	"fa/pkgname2"
	fbpkg2 "fb/pkgname2" // 重命名导入，避免歧义
)
import _ "png" // 空白导入。代码不引用png包，但是需要对png包级别变量初始化表达式求值，并执行其init函数。编译器会链接对应函数。一般用于以插件方式提供某些功能支持（如文件格式，数据库驱动等)。
```

- 命名建议：简短可读，含义明确，无歧义，形式统一；与成员名字的配合，重要成员名字尽量简短；必要时使用New函数；
- go工具：go help 通过子命令实现下载、查询、格式化、构建、测试、包管理
```
go工具：
build, clean, doc, env, bug, fix, fmt, generate, get, install, list, run, test, tool, version, vet
	build  构建并生成exe，然后丢弃除exe的部分 -i 安装目标程序的依赖
    install  类似build，但是保存编译后的每一个包到$GOPATH中(pkg，bin)
    run  使用第一个不是.go结尾的参数及之后的部分，作为编译后可执行程序的参数
    
附加帮助主题：
c, buildmode, cache, filetype, gopath, environment, importpath, packages, testflag, testfunc
```

- 包结构: go help gopath

  src：源码，每个包一个文件夹

  pkg：编译后的包

  bin：构建后的程序

  xxx_arch.go: 对应体系结构代码

  // +build xxx: 构建标签，参考go/build Build Constraints

- 包的文档化：包声明、导出成员应加注释。可参考go/src风格。

  可以使用单独的注释文件，如go/src/fmt/doc.go

  启动本地文档服务：`godoc -http :8000`可使用 `http://127.0.0.1:8000`浏览文档

- 内部包：internal目录下的包只能被internal所在目录下的包引用

```
net/http
net/http/internal/chunked
net/http/httputil
net/url

chunked可以被http或httputil引用，不能被url引用
但url可以引用httputil
```

- 包的查询：`go list` 通配符 `go list ...xml...`

# 5. 遇到的问题

## 5.1 运行代码

类似VS，调试F5, 非调试运行Ctrl+F5. 也可以终端输入命令：go build然后运行命令。注意  `go build | bin.exe`并不能执行最新编译后的程序，而是运行的上次编译的程序。推测和windows系统预取优化有关。

## 5.2 用户的package




# 6. 参考资源

https://go-zh.org

https://tour.go-zh.org







