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

左侧菜单栏：文件，搜索，SCM（内部集成git），调试，扩展

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

7. **安装常用go插件**

   新建并打开文件夹(go项目文件夹)，新建xx.go文件。随便写一些代码，保存，系统会提示是安装某些插件。点install all即可自动安装。插件包括：

```
gocode gopkgs go-outline go-symbols guru gorename godef goreturns golint dlv
gomodifytags goplay impl fillstruct godoc gotests
```
8. **配置调试参数**
  调试，打开配置 / 添加配置
```
"program" 调试的程序，可以是"${fileDirname}"  "${workspaceFolder}"，并可使用相对路径
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

代码：

```
fmt.Errorf：strings should not be capitalized or end with punctuation or a newline
	不应使用大写，末尾不能加 . 或 \n。因为返回的字串经常在是其他格式化的一部分。但是大写不会导致报错，末尾有.或\n会报错。	
```





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

# 7. 翻译参考

## 7.1 go help gopath

```
The Go path is used to resolve import statements.It is implemented by and documented in the go/build package.
Go path用来解析导入语句。实现及文档参考go/build包

The GOPATH environment variable lists places to look for Go code. On Unix, the value is a colon-separated string. On Windows, the value is a semicolon-separated string. On Plan 9, the value is a list.
环境变量GOPATH列出了go代码的搜索位置。UNIX下是逗号分隔的字串，Windows下是分号分隔的字串，Plan 9下是一个列表。

If the environment variable is unset, GOPATH defaults to a subdirectory named "go" in the user's home directory ($HOME/go on Unix, %USERPROFILE%\go on Windows), unless that directory holds a Go distribution. Run "go env GOPATH" to see the current GOPATH.
如果环境变量未设置，除非目录中包含go发布版，默认为用户home目录下go子目录（Unix: $HOME/go, WIndows: %USERPROFILE%\go)。查看命令"go env GOPATH"

See https://golang.org/wiki/SettingGOPATH to set a custom GOPATH.
自定义GOPATH参考https://golang.org/wiki/SettingGOPATH

Each directory listed in GOPATH must have a prescribed structure:
GOPATH中的每一个目录应该有如下预定义结构：

The src directory holds source code. The path below src determines the import path or executable name.
src目录保存源码。src下的路径决定导入路径或可执行文件名

The pkg directory holds installed package objects. As in the Go tree, each target operating system and architecture pair has its own subdirectory of pkg
(pkg/GOOS_GOARCH).
pkg目录保存安装的包对象。在Go树中，每一个OS和体系结构对具有对应包的子目录（pkg/GOOS_GOARCH)

If DIR is a directory listed in the GOPATH, a package with source in DIR/src/foo/bar can be imported as "foo/bar" and has its compiled form installed to "DIR/pkg/GOOS_GOARCH/foo/bar.a".
如果DIR是GOPATH中的一个目录，源码DIR/src/foo/bar中的包可以使用导入名"foo/bar"，编译后安装位置为"DIR/pkg/GOOS_GOARCH/foo/bar.a"

The bin directory holds compiled commands. Each command is named for its source directory, but only the final element, not the entire path. That is, the command with source in DIR/src/foo/quux is installed into DIR/bin/quux, not DIR/bin/foo/quux. The "foo/" prefix is stripped so that you can add DIR/bin to your PATH to get at the installed commands. If the GOBIN environment variable is set, commands are installed to the directory it names instead of DIR/bin. GOBIN must be an absolute path.
bin目录保存编译后的命令。方便起见，命令名由源码路径最后一部分决定，而非整个路径。即DIR/src/foo/quux生成DIR/bin/quux，而非DIR/bin/foo/quux。...如果设置了环境变量GOBIN，命令将会安装在设置的路径。GOBIN必须为绝对路径。

Here's an example directory layout:

    GOPATH=/home/user/go

    /home/user/go/
        src/
            foo/
                bar/               (go code in package bar)
                    x.go
                quux/              (go code in package main)
                    y.go
        bin/
            quux                   (installed command)
        pkg/
            linux_amd64/
                foo/
                    bar.a          (installed package object)

Go searches each directory listed in GOPATH to find source code, but new packages are always downloaded into the first directory in the list.
Go会搜索GOPATH中的每一个文件夹以查找源码，但是新包都会下载到列表中的第一个目录。

See https://golang.org/doc/code.html for an example.

Internal Directories
Internal目录

Code in or below a directory named "internal" is importable only by code in the directory tree rooted at the parent of "internal". 
internal目录中的代码只能被internal所在目录中的包导入
Here's an extended version of the directory layout above:

    /home/user/go/
        src/
            crash/
                bang/              (go code in package bang)
                    b.go
            foo/                   (go code in package foo)
                f.go
                bar/               (go code in package bar)
                    x.go
                internal/
                    baz/           (go code in package baz)
                        z.go
                quux/              (go code in package main)
                    y.go


The code in z.go is imported as "foo/internal/baz", but that
import statement can only appear in source files in the subtree
rooted at foo. The source files foo/f.go, foo/bar/x.go, and
foo/quux/y.go can all import "foo/internal/baz", but the source file
crash/bang/b.go cannot.

See https://golang.org/s/go14internal for details.

Vendor Directories
Vendor目录

Go 1.6 includes support for using local copies of external dependencies to satisfy imports of those dependencies, often referred to as vendoring.
GO 1.6支持使用外部依赖的本地拷贝解决依赖版本问题。

Code below a directory named "vendor" is importable only by code in the directory tree rooted at the parent of "vendor", and only using an import path that omits the prefix up to and including the vendor element.
vender目录中的包只能被vender所在目录中的包导入，导入路径会忽略vender元素前的部分。

Here's the example from the previous section, ut with the "internal" directory renamed to "vendor" and a new foo/vendor/crash/bang directory added:

    /home/user/go/
        src/
            crash/
                bang/              (go code in package bang)
                    b.go
            foo/                   (go code in package foo)
                f.go
                bar/               (go code in package bar)
                    x.go
                vendor/
                    crash/
                        bang/      (go code in package bang)
                            b.go
                    baz/           (go code in package baz)
                        z.go
                quux/              (go code in package main)
                    y.go

The same visibility rules apply as for internal, but the code in z.go is imported as "baz", not as "foo/vendor/baz".
包可见性类似internal，但是z.go导入名是"baz"，而非"foo/vendor/baz"。

Code in vendor directories deeper in the source tree shadows code in higher directories. Within the subtree rooted at foo, an import of "crash/bang" resolves to "foo/vendor/crash/bang", not the top-level "crash/bang".
vender目录中的导入包会增加vender部分的前缀。如"crash/bang"会解析为"foo/vendor/crash/bang"，而非顶级目录中的"crash/bang"。

Code in vendor directories is not subject to import path checking (see 'go help importpath').
vender目录中的代码不会做导入路径检查。参考'go help importpath'。

When 'go get' checks out or updates a git repository, it now also updates submodules.
go get签出或更新git仓库是，也会更新子模块。

Vendor directories do not affect the placement of new repositories being checked out for the first time by 'go get': those are always placed in the main GOPATH, never in a vendor subtree.
vender目录不会影响'go get'第一次迁出的新仓库的存放，这些仓库总是存放在主GOPATH中，而非vender子目录中。

See https://golang.org/s/go15vendor for details.

```





