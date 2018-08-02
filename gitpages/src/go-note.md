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
？？？"cwd": "${workspaceRoot}" 运行目录绝对路径
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

## 2.2 查找引用

goru, go-find-references

goru比go-find-references慢，但更准确：https://github.com/Microsoft/vscode-go/issues/340

vscode go项目比较大时，find all refernces可能耗时十几秒https://github.com/Microsoft/vscode-go/issues/1491

为了提升速度，可以考虑使用[Go Language Server](https://github.com/Microsoft/vscode-go/blob/master/README.md#go-language-server-experimental)，但目前无windows支持。

但是，都不好用！！！如果项目没有加入到GOPATH，就会无结果。现在不知道是不是guru本来就有这个问题，还是安装Language Server引起的，卸载后问题仍然存在。

老老实实用GoLand吧

## 2.3 快捷键

设置(macOS):

```
code，首选项，键盘快捷方式
可以输入快捷键搜索，如"ctrl alt cmd shift"，或者目标如"go.test"
```

默认快捷键：

```
后退 ^-
前进 ^ Shift -
命令面板 cmd shift p 或 F1
	? 查询
	> 命令
	@ 查找符号
	直接输入： 打开文件名
```



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

# 5. Go tour

`go get github.com/Go-zh/tour/gotour`并执行GOPATH中的`gotour`

- [ ] Go tour英文版
- [ ] Go tour练习

## 5.1 包、变量和函数

[Go 语法声明：为何类型在名称之后](http://blog.go-zh.org/gos-declaration-syntax)

```
约定：包名与导入路径的最后一个元素一致
导出：大写字母开头。“未导出”的名字在该包外均无法访问
连续命名：x,y int
函数接收者：receiver，即函数接受的参数（入参）
return：多返回值按顺序赋值。直接返回：返回已命名的返回值（影响代码可读性）。
声明：var。可以赋初值。短声明:=。
基本类型：bool string int(8,16,32,64) uint(8,16,32,64,uintptr) byte=uint8 rune=int32(unicode) float32/64 complex64/128 
类型转换及推导：必须显式转换。根据形式及精度推导（complex g = 0.1 + 5i)
常量：const，不能用:=
数值型常量：高精度值。如const Bit = 1 << 100
```

## 5.2 流程控制

[defer panic  and recover](http://blog.go-zh.org/defer-panic-and-recover)

```
for [初始化][;][条件][;][后置]
	for i := 0; i < 10; i++ { ... // i仅for段可见。
	for ; i < 10; { ...
	for i < 10 { ...
	for { ... // 无限循环
if [初始化;] 条件
	if v := math.Pow(x, n); v < lim { ... // v仅if-else段可见。初始化可省略
	} else { ... // 注意else前后大括号格式
switch [初始化;] 判定值 
	case不要求常量、整数，可以为函数等
    只执行选定case（自动break），除非显式fallthrough
    依次执行case，直到匹配为止（可能跳过后续case调用）
    无判定值，则为true，可用于简化if-else
defer 延迟函数执行到外层函数返回之后。立即求值。多个defer使用栈，LIFO调用。
label: 用于goto, break, continue
	1. 用于goto无限制
	2. 在for前面，用于break或者continue
	3. 在switch前面，用于break
```

## 5.3 更多类型：struct、slice和map

 [Go 切片：用法和本质](https://blog.go-zh.org/go-slices-usage-and-internals)

- [ ] [Golang struct、interface 组合嵌入类型详解](https://www.jianshu.com/p/d87c69ac6ce7)

```
指针：*T nil & * 无指针运算
结构体：字段名 类型名 [可选标签字串]
	t = T{1,2,"asdf"}
	p = &T{c:"asdf", a:1}
	var p *T = new(T)
	点号访问。指针访问(*p).x或p.x。顺序赋值，或Name:指定字段名。&S{...}返回结构体指针。
	匿名/内嵌字段：只有类型没有名字，类型名就是字段名。行为类似继承，外层结构体可以直接访问内层的变量和方法。
	reflect.TypeOf严格匹配类型，不会做类似基类-子类的转化
	命名冲突：外层覆盖内层（类似重载）。同层报错。
数组：[n]T  n不可更改，可推算时可忽略（如赋初值，返回切片）
切片：array[low:high] 选定半开区间[low,high) low或high可省略，默认为0和数组长度 
	返回数组部分元素的引用（原数组可被返回的数组修改）
	可求len，cap，零值为nil
	可扩展（在原数组上扩展）
	make（[]int, len [, cap]): 创建数组并返回切片
	append追加元素到切片（非数组）末尾，需要时可自动扩展原数组。
range：遍历数切片或映射，返回下标及对应元素的副本。_ 忽略对应返回值
map：nil。必须有键名。顶级类型名赋值中可省略。
	遍历: for k,v range mapVar {}
	遍历key: for k range mapVar {}
	遍历value: for _, v range mapVar {}
	插入、修改或获取直接下标。下标不存在时返回元素零值
	删除用delete函数
	双赋值检测key是否存在： elem, ok := m[key]
函数值：类似函数指针
	func compute(fn func(float64, float64) float64) float64 {
		return fn(3, 4)
	}
**函数的闭包：闭包是一个函数值，它引用了其函数体之外的变量。该函数可以访问并赋予其引用的变量的值。	
```

## 5.4 方法和接口

本地类型：local type。包内定义的类型。方法及入参指针只能使用本地类型。type MyFloat float64在包内定义了类型MyFloat	

```
方法：以结构体为参数的函数。 func (v Vertex) Abs() float64 { ... // v.Abs()
				   函数: func Abs (v Vertex) float64 { ...   // Abs(v)
	可以为本地类型非结构体类型声明方法。
指针：避免传值调用时的副本拷贝，直接修改源值。只能是本地类型。
	方法涉及到的指针（指针接收者或者对象指针）可以默认转换，函数不可以，必须显式指定
	pv.Abs()可以解释为(*pv).Abs()
	Abs(v)可以，但是Abs(pv)会报错
	
	func (v *Vertex) PAbs()
	func PAbs(v *Vertex, f float64)
    方法可以直接调用 v.PAbs() -> (&v).PAbs() 
    函数必须显式使用指针 PAbs(&v)
接口：接口类型是由一组方法签名定义的集合。类型通过实现一个接口的所有方法来实现该接口。空接口（0个方法）
	type Abser interface{
        Abs() float64
	}	
	var a Abserj
	// a = ...
	a.Abs()
类型断言：访问接口底层具体值。  t := i.(T) (类型不匹配会panic)   t, ok := i.(T)
类型选择：按顺序从几个类型断言中选择分支
	swtch v := i.(type) {
        case T1:
        	// ...
        case T2:
        	// ...
        default:
        	// ...
	}
Stringer: fmt包中的接口，使用字符串描述自身，用于fmt打印
	type Stringer interface {
        String() string
    }
error: 错误状态
    type error interface {
        Error() string
    }
io.go: 一组io接口
	Read(b []byte) (n int, err error)
	Write(p []byte) (n int, err error)
	...
image.go: 图像接口
    type Image interface {
        ColorModel() color.Model
        Bounds() Rectangle
        At(x, y int) color.Color
    }
```

## 5.5 并发

```
goroutine:
	go f(x,y,z) 启动一个新的goroutine比你高执行 f(x,y,z)
	sync做共享保护
channel:
	ch := make(chan int [,buflen])	// 创建信道，可选缓冲长度
	ch <- v		// v发送到信道ch
	v := <- ch	// 从ch接收并赋予v
	缓冲区满会阻塞。缓冲区不够可能死锁。
	range ch：不断从信道ch接收
	close(ch): 关闭信道。一般信道不需要关闭，除非有必要（如结束range ch）
	select：可实现阻塞等待ch，当多个ch可用时随机选择。可搭配default处理全阻塞时的情况。
sync.Mutex: 有Lock和Unlock方法。
```

## 5.6 其他

反射

```
type reflectInterface struct{}

func (t *reflectInterface) ShowMsg() {
	fmt.Println("reflectInterface.showMsg called.")
}

func reflectSample() {
	// MethodByName: export method only
	v := reflect.ValueOf(&reflectInterface{}).MethodByName("ShowMsg")
	if v.Kind() != reflect.Func {
		fmt.Println("no such method")
	} else {
		v.Call([]reflect.Value{})  // parameters can packed into []reflect.Value{}
	}
}
```



# 6. 遇到的问题

## 6.1 运行代码

类似VS，调试F5, 非调试运行Ctrl+F5. 也可以终端输入命令：go build然后运行命令，或者`go run main.go`。注意  `go build | bin.exe`并不能执行最新编译后的程序，而是运行的上次编译的程序。推测和windows系统预取优化有关。

## 6.2 用户的package

参考9.1 `go help path`

```
一个目录中只能有一个package
一般代码放在src路径下。如果引用其他src路径，需要将src上级目录添加到gopath中。vscode的gopath分用户和项目两级，和系统的gopath互不影响。更改vacode的gopath可能导致插件重新安装
```

# 7. 代码笔记

## 7.1 常用库 

### string int int64互转

```
#string到int  
int,err:=strconv.Atoi(string)  
#string到int64  
int64, err := strconv.ParseInt(string, 10, 64)  
#int到string  
string:=strconv.Itoa(int)  
#int64到string  
string:=strconv.FormatInt(int64,10)  
```

### string, []byte, 结构体互转

```
type strudef struct{
    Item string `json:"field_name"`
}

	stru := &strudef{"fieldvalue"}
	b, e := json.Marshal(stru)
	s := string(b)

	var fmtJSON bytes.Buffer
	e2 := json.Indent(&fmtJSON, b, "", "  ")
	s2 := string(fmtJSON.Bytes())
```

### error

```
https://gobyexample.com/errors
var err error = errors.New("xxx")
err = fmt.Errorf("%s %d", xxx, nnn)
实现一个具有Error() string{}方法的结构体，直接赋值给err
```

## 7.2 语言特性

### 接口

- [ ] [Golang面向接口编程](https://blog.csdn.net/huwh_/article/details/79054450)
- [ ] [理解 Go interface 的 5 个关键点](http://sanyuesha.com/2017/07/22/how-to-understand-go-interface/)

# 8. 参考资源

https://go-zh.org

https://tour.go-zh.org

# 9. 翻译参考

## 9.1 go help gopath

gopath需要用绝对路径，可以多个

检索import包时，按照gopath中顺序检索。一般是在src上一目录作为gopath路径。

```
The Go path is used to resolve import statements.It is implemented by and documented in the go/build package.
Go path用来解析导入语句。实现及文档参考go/build包

The GOPATH environment variable lists places to look for Go code. On Unix, the value is a colon-separated string. On Windows, the value is a semicolon-separated string. On Plan 9, the value is a list.
环境变量GOPATH列出了go代码的搜索位置。UNIX下是逗号分隔的字串，Windows下是分号分隔的字串，Plan 9下是一个列表。MAC下是冒号分割的字串。

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





