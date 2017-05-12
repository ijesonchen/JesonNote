---
title: hexo how-to：github+hexo实现个人博客
date: 2017-04-17 00:01:23
tags: [hexo, how-to]
---

# 参考资料
所有资料来源于网上
<!--more-->

[用Hexo创建个人博客](http://www.jianshu.com/p/b06222fbc135)
[手把手教你使用Hexo + Github Pages搭建个人独立博客](https://segmentfault.com/a/1190000004947261)

# 准备

[Git](https://git-scm.com/downloads)
代码管理、GitBash.
下载对应版本

[Node.js](https://nodejs.org/en/download/)
hexo依赖的框架
下载对应版本

[Hexo](https://hexo.io/)
[主题](https://hexo.io/themes/)
hexo框架及主题介绍


# 安装

1. 安装git-gui
2. 安装Node.js
3. 运行GitBash(git-gui会添加右键菜单)，进入命令行

安装hexo(需要等待)
`npm install hexo-cli -g`
`npm install hexo -g` 也可以？

显示hexo版本
`hexo -v`

```
$ hexo -v
hexo-cli: 1.0.2
os: Windows_NT 6.1.7601 win32 x64
http_parser: 2.7.0
node: 6.9.5
v8: 5.1.281.89
uv: 1.9.1
zlib: 1.2.8
ares: 1.10.1-DEV
icu: 57.1
modules: 48
openssl: 1.0.2k
```

# Hexo的使用
主要步骤：
- hexo n 写文章
- hexo g 生成页面
- hexo s 本地预览(可省略), `Ctrl+C`结束
- hexo d 自动部署（需配置）

可以合并操作，例如`hexo s -g`表示先生成再预览，`hexo d -g`表示先生成再部署

## 常用命令
```hexo
$ hexo n == hexo new
$ hexo g == hexo generate
$ hexo s == hexo server
$ hexo d == hexo deploy
```

### 如何写文章

#### 1. 初始化工作区
  hexo init
  如未初始化，则会显示hexo的命令说明而非工作区未初始化:
  ```cli
  Usage: hexo <command>
...
```

#### 2. 创建文章
  hexo n "title"

#### 3. 使用markdown编辑md文件
文件头示例：
```language
---
title: hexo how to：github+hexo实现个人博客
date: 2017-04-16 22:26:15
tags: [hexo, how-to]
---
```
https://github.com/hexojs/hexo/issues/320
tags采用[YAML](https://en.wikipedia.org/wiki/YAML)数组格式，可以使用 [ , ] 或 - 来分割：
```language
tags:
- tag1
- tag2
```

#### 4. 生成页面
  hexo g

#### 5. 预览
  hexo s

#### 6. 配置

#### 7. 主题
  [Maupassant](https://github.com/tufu9441/maupassant-hexo)
  简介、支持图片和代码、带评论功能
  `npm install hexo-renderer-sass --save`失败
  推测可能是因为环境变量中未找到python
  ```cli
gyp verb check python checking for Python executable "python2" in the PATH
gyp verb `which` failed Error: not found: python2
...
gyp verb check python checking for Python executable "python" in the PATH
gyp verb `which` succeeded python C:\Program Files\Python35\python.EXE
gyp verb check python version `C:\Program Files\Python35\python.EXE -c "import platform; print(platform.python_version());"` returned: "3.5.1\r\n"
gyp verb could not find "C:\Program Files\Python35\python.EXE". checking python launcher
gyp verb could not find "C:\Program Files\Python35\python.EXE". guessing location
gyp verb ensuring that file exists: C:\Python27\python.exe
gyp ERR! configure error
gyp ERR! stack Error: Can't find Python executable "C:\Program Files\Python35\python.EXE", you can set the PYTHON env variable.
...
```
  解决：
- bash环境变量中添加python 3.5
  	http://www.cnblogs.com/amboyna/archive/2008/03/08/1096024.html
  	export PATH=$PATH:"python 3.5 python"
- 设置npm代理(加速下载)
  	npm config set proxy http://server:port
  官方给出的提示：
```
若npm install hexo-renderer-sass安装时报错，可能是国内网络问题，请尝试使用代理或者切换至淘宝NPM镜像安装，感谢光头强提供的方法。
```

#### 8. 部署
https://hexo.io/zh-cn/docs/deployment.html
可以使用 `hexo d -g` 同时完成生成和部署

```language
Hexo 提供了快速方便的一键部署功能，让您只需一条命令就能将网站部署到服务器上。
$ hexo deploy
在开始之前，您必须先在 _config.yml 中修改参数，一个正确的部署配置中至少要有 type 参数，例如：
deploy:
  type: git
您可同时使用多个 deployer，Hexo 会依照顺序执行每个 deployer。
deploy:
- type: git
  repo:
- type: heroku
  repo:
```

- 安装hexo-deployer-git
  `npm install hexo-deployer-git --save`
- 修改配置_config.yml

```language
deploy:
  type: git
  repo: <repository url>
  branch: [branch]
  message: [message]
```

```language
参数	描述
repo	库（Repository）地址
branch	分支名称。如果您使用的是 GitHub 或 GitCafe 的话，程序会尝试自动检测。
message	自定义提交信息 (默认为 Site updated: {{ now('YYYY-MM-DD HH:mm:ss') }})
```

#### 9. Github Pages
https://pages.github.com/
https://help.github.com/articles/user-organization-and-project-pages/
repo: git@github.com:{username}/{username}.github.io.git
访问：{username}.github.io


### 其他
[将原始的 .md 文件纳入 hexo 的版本管理](http://baurine.github.io/2015/05/10/hexo_git.html)

windows命令行接收输入参数：
@echo off
echo input target dir:
set /p var=
dir %var%