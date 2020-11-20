---
title: git note
date: 2017-05-24 22:14:00
tags: [git]
---



[TOC]



# 0. 安装

```
windows: 下载安装包
ubuntu：apt install git
centos：yum install git

源码安装(centos 7.4 示例）：
wget https://mirrors.edge.kernel.org/pub/software/scm/git/git-2.9.5.tar.gz
tar -zxvf git-2.9.5.tar.gz
cd git-2.9.5
./configure
make -j64 ## 64并发编译
make install
可以指定目录：
# make prefix=/usr/local/git all
# make prefix=/usr/local/git install

****如果提示错误
    SUBDIR perl
/usr/bin/perl Makefile.PL PREFIX='/usr/local' INSTALL_BASE='' --localedir='/usr/local/share/locale'
Can't locate ExtUtils/MakeMaker.pm in @INC (@INC contains: /usr/local/lib64/perl5 /usr/local/share/perl5 /usr/lib64/perl5/vendor_perl /usr/share/perl5/vendor_perl /usr/lib64/perl5 /usr/share/perl5 .) at Makefile.PL line 3.
BEGIN failed--compilation aborted at Makefile.PL line 3.
make[1]: *** [perl.mak] Error 2
make: *** [perl/perl.mak] Error 2

说明缺少ExtUtils/MakeMaker.pm
解决
1. yum install perl-devel
或者直接
2. yum install perl-ExtUtils-MakeMaker 

****如果提示 MSGFMT tclsh相关的错误
yum install gettext
即可。
其他可能缺少的包
# yum install curl-devel expat-devel gettext-devel openssl-devel zlib-devel asciidoc
# yum install  gcc perl-ExtUtils-MakeMaker
```




# 1. 使用git本地版本库实现多地同步

## 应用场景
一些笔记及音视频资料，在家和公司都可能增删文件，但是比较大，希望通过U盘作为中介同步。
<!--more-->

## 具体步骤
使用tortoise git工具。估计这就是git的常规用法之一。其实就是用u盘作为远程git库载体。

1. u盘新建 datasync.git 文件夹，并创建为本地Bare库(用于同步数据，仅支持push)
2. 公司电脑创建 datawork 文件夹，创建为本地git库（非bare）,添加文件并提交到master
3. datawork， push到Arbitrary Url，填写u盘路径如 x:\datasync.git
4. 个人电脑 git clone x:\datasync.git,为方便区分，文件夹命名为 datahome
5. 可以在 datawork 和 datahome 中增删文件，需要同步时push到 datasync.git，在另一个文件夹中pull

## 问题

- ### push时，音视频文件compress耗时较长

#### 原因
delta diff大文件耗时
#### 参考
[git-doc-gitattributes](https://git-scm.com/docs/gitattributes)
Packing objects
delta
Delta compression will not be attempted for blobs for paths with the attribute delta set to false.

[git pull without remotely compressing objects](https://stackoverflow.com/questions/7102053/git-pull-without-remotely-compressing-objects)

Git does spend a fair bit of time in zlib for some workloads, but it should not create problems on the order of minutes.

For pushing and pulling, you're probably seeing delta compression, which can be slow for large files

core.compression 0 # Didn't seem to work.
That should disable zlib compression of loose objects and objects within packfiles. It can save a little time for objects which won't compress, but you will lose the size benefits for any text files.

But it won't turn off delta compression, which is what the "compressing..." phase during push and pull is doing. And which is much more likely the cause of slowness.

pack.window 0
It sets the number of other objects git will consider when doing delta compression. Setting it low should improve your push/pull times. But you will lose the substantial benefit of delta-compression of your non-image files (and git's meta objects). So the "-delta" option above for specific files is a much better solution.

echo '*.jpg -delta' > .gitattributes
Also, consider repacking your repository, which will generate a packfile that will be re-used during push and pull.

#### 解决
使用 -delta属性禁止某些文件的delta diff操作: datawork根目录下新建 .gitattributes 文件，添加如下内容
```git-attributes
*.mp4 -delta
*.zip -delta
*.pdf -delta
```

# 2. git只签出某一子目录

git本身建议这样操作。建议使用submodule，但是有类似的实现方式："sparse clone" and "sparse fetch" 

[git只clone仓库中指定子目录和指定文件的实现](http://blog.csdn.net/xuyaqun/article/details/49275477)

# 3. github

## 3.1常规操作

```
clone: 需要本地编译、修改等。不要下载zip文件，因为不会迁出子项目
star：点赞，可以在your stars看到曾经点赞过的项目，类似收藏。
watch：项目有提交等变化时收到邮件提醒。 
fork：自己需要进行一些修改。不会和原项目自动同步。一般是修改后提出pull request到原作者，经同意后合并到原仓库。
建议：喜欢的项目star（收藏），需要跟进watch（修改邮件通知），想要贡献fork（测过后提pull request）
```



## 3.2 fork项目后的同步

目前不支持自动同步。需要手动操作，create pull request或者命令行实现

## 3.3 跟踪项目

不建议fork，建议star相关项目，之后可以在your star里面看到。

## 3.4 获取ssh pubkey

```
curl https://github.com/<user-account>.keys
即可得到用户user-account的ssh pubkey。将这些key放入Linlux <home>/.ssh/authorized_keys既可支持服务器远程登录
```

## 3.5 临时指定配置参数

```
--global指定全局参数
-c 指定本次执行参数
git -c core.fileMode=false diff
```

## 3.6 加速

clone加速-国内镜像

```
gitclone.com

https://gitclone.com ，目前mirror了一万多个代码库，star超过1500的都做了镜像。通过gitclone.com clone，如果能命中镜像，则速度会达到1M多，如果之前未mirror过，会随着git clone命令建立起镜像，下次clone会用到。使用起来很简单，URL稍微改一下，如 git clone "https://gitclone.com/github.com/xxx/yyy">https://gitclone.com/github.com/xxx/yyy ，或干脆不改url，如 cgit clone https://github.com/xxx/yyy ，不需要fork、不需要登录。
链接：https://www.zhihu.com/question/38192507/answer/1292617132
```


下载加速
```
http://toolwa.com/github/


https://niao.su/7/
GitHub国内加速下载
GitHub我们都知道是世界上最大的开源及私有软件项目的托管平台，全世界每天有海量优秀的开源软件在这里产生，而GitHub在国内很多时候获取到的下载链接是亚马逊的服务器。
中国因为不可言说的原因，经常抽疯或龟速。想要加快GitHub下载速度就需要用到GitHub国内加速服务，对于有条件的可以使用代理加快访问速度，而没有条件的就可以用到网上热心人士维护的加速服务了。
GitHub hosts
https://github.com/521xueweihan/GitHub520
通过修改hosts加快GitHub访问速度，日常频率使用高的可以用这个方法。
GitHub镜像地址
* https://github.com.cnpmjs.org
* https://hub.fastgit.org
github加速下载
http://toolwa.com/github/   
https://d.serctl.com/         ***********
GitHub文件加速
利用 Cloudflare Workers 对 github release 、archive 以及项目文件进行加速，部署无需服务器且自带 cdn
https://gh.api.99988866.xyz
https://g.ioiox.com
http://gitd.cc
https://gh.sky-and-poem.fun
该网站为演示站点，如无法打开可以查看开源项目：gh-proxy-GitHub文件加速自行部署。
加速你的Github
https://github.zhlh6.cn
输入Github仓库地址，使用生成的地址进行git ssh操作即可
GitHub加速谷歌浏览器扩展
.crx下载
如果可以直接访问谷歌商店，可以访问GitHub加速谷歌商店安装。
GitHub加速插件
油猴插件：加速下载脚本
在此之前需要安装油猴或暴力猴扩展，如已安装请忽视
暴力猴
油猴
crx下载
如无法直接安装.crx扩展，使用终极解决方法，把.crx解压缩，然后在扩展中心中开启 开发者模式然后选择加载已解压的扩展程序。
GitHub缓存加速网站
https://gitclone.com/
为开发者服务（使用git2.0+）只需在git clone命令中将gitclone.com嵌入到克隆地址中即可
GitHub raw加速
GitHub raw域名并非github.com 而是raw.githubusercontent.com，上方的GitHub加速并不能加速这个域名，那么可以使用Static CDN提供的反代服务。
将raw.githubusercontent.com替换为raw.staticdn.net 即可加速。
GitHub + Jsdelivr
jsdelivr 唯一美中不足的就是它不能获取 exe文件以及 Release 处附加的exe和dmg文件。
也就是说如果exe文件是附加在 Release 处但是没有在 code 里面的话是无法获取的。所以只能当作静态文件cdn用途，而不能作为Release加速下载的用途。
来源：Hunsh's Blog、GitHub中转下载、Jsdelivr+GitHub加速
```



# 4. Git常用工作流程

## 4.1 工作流：github

```
首先，git clone <repo> 克隆repo
1. git fetch <remote> // 更新repo
2. git checkout -b <local_branch_name> <remote_base_branch> // 根据远程base创建分支，一般是origin:master
3. working & commit
4. git fetch <remote> & git rebase <remote_base_branch> // 保持和远程分支的同步
5. git push <remote> <local_branch_name>:<remote_branch_name> // 提交分支到远程
6. 提交remote_branch_name到remote_base_branch 的pr或mr，同时申请review
7. 如果需要修改，本地修改后，git push
8. 合并到base_branch
```



## 4.2 工作流：gitlab

## 4.3 常用命令

help

```
git help
git cmd --help // 如git branch --help
```

git checkout

```
git checkout <branch name> // checkout分支
git checkout -b <branch name> [remote branch]// 创建并co分支。注意不是-B
	git checkout -b localBranchName remoteRepo/remoteBranchName
```

Git branch

```
git branch // 查看本地分支
git branch -a // 查看所有分支（含remote）
git branch -d <branch name> // 删除分支。如果含中文，特殊符号等，可以加引号
```

git log / reflow

```
git log // 显示修改日志
git reflog // 显示所有修改日志，包括reset操作。但是git branch -d之后无法显示
```

git show

```
git show hashid	// 显示提交的更改
git show hashid --name-only // 仅显示提交修改的文件名
git show --pretty="" --name-only hashid
```

git reset

```
git reset hashid // 撤销修改到hashid，之后的commit会撤销，但是修改回保留，变成未提交状态
```

清除所有未提交的文件及文件夹

```
git checkout . #本地所有修改的。没有的提交的，都返回到原来的状态
git stash #把所有没有提交的修改暂存到stash里面。可用git stash pop恢复。
git reset --hard HASH #返回到某个节点，不保留修改。
git reset --soft HASH #返回到某个节点。保留修改

git clean -df #返回到个节点
git clean 参数
    -n 显示 将要 删除的 文件 和  目录
    -f 删除 文件
    -df 删除 文件 和 目录
    -x 删除所有文件（包括 .gitignore中的文件）
git checkout . && git clean -xdf
```

新建分支处理issue

```
1. repo: user, upstream。github上user fork upstream的repo
2. git clone user/repo
3. 本地git添加remote： git remote add upstream [upstream/repo]
4. github上，upstream创建issue分支 issue001
5. 本地签出upstream的分支
	git fetch upstream
	git checkout -b issue001 upstream/issue001
6. 本地修改。测试。完成后做rebase
7. 提交到user的repo
	git push origin issue001
8. github上面提交pr: user/issue001 -> upstream/issue001。review后，merge分支。测试发布
9. 测试通过后，提交pr: upstream/issue001 -> upsteram/master。由管理员review，merge
```



同步fork和upstream的代码

```
本地git clone fork后的项目
1. 添加远程原始库
	git remote add upstream git@github.com:originuser/originrepo.git
	git remote -v 可以看到是否添加成功
2. master拉取upstream，合并，push
	git checkoiut master
	git fetch upstream
	git merge upstream/master
	git push origin master
3. 分支rebase
	git checkout mybranch
	git rebase master
```

### 获取信息

```shell
git symbolic-ref --short HEAD  // 本地分支名
git rev-parse <branch-name> //  分支id?
git rev-parse --symbolic-full-name <branch-name>@{u}  // 分支对齐？
```

### 其他

```
ssh key fingerprint：
ssh-keygen -E md5 -lf id_rsa.pub  // md5方式
```



## 4.4 常用操作

### 强制使用远程分支覆盖本地

```
git fetch --all
git reset --hard origin/master
git pull
```

### 清理reflog

```
git reflog expire --expire=now --all
git gc --prune=now
```

### 清理错误提交的大文件

```
http://liuhui998.com/2010/11/06/remove_commits_completely/
如何彻底删除 Git 中的提交(commit)

git reset --hard <old-commit> // 回退到之前的提交
git reflog expire --expire-unreachable=0 --all // 清理reflog
git gc --prune=0 // 触发gc
```



### 初始化库

```
你有一个空的存储库
To get started you will need to run these commands in your terminal.
 
第一次配置Git
git config --global user.name "your_name"
git config --global user.email "your_mail"
使用您的存储库
我只想克隆这个存储库
如果要简单地克隆此空存储库，请在终端中运行此命令。

git clone <git-url>
我的代码已经准备好推送
如果你代码已经准备好推送到仓库，请在终端中执行该命令

cd existing-project
git init
git add --all
git commit -m "Initial Commit"
git remote add origin <git-url>
git push -u origin master
我的代码已经由Git跟踪
如果你的代码已经由Git跟踪，然后设置这个仓库作为你的“origin”推送。

cd existing-project
git remote set-url origin <git-url>
git push -u origin --all
git push origin --tags
所有用命令完成？
```



## 4.5 QA

1）commit your changes or stash them before you can merge.

```
情景：在master分支上修改完未提交，发现需要在分支b1上提交，master改变
git stash // 备份当前分区修改
git checkout b1 // 签出b1分支
git stash pop // 恢复修改。后续可以继续在b1分支上工作
```

2)  fatal: Couldn't look up commit object for HEAD

```
情景：误操作 git check -b <branch name> 导致branch name为中文含：。git branch显示不正常，分支列表全部为白色，无星号（正常显示是branch列表，且当前分支为绿色标*）。git branch -d删除其他分支正常，删除中文名的分支报错。
解决：git checkout 切换到master分支后，恢复正常。git branch -d "中文分支名"成功
```

3）换行符问题

```
windows crlf 
unix/linux lf
mac cr

git可配置为自动对换行符进行转换
.gitconfig
[core]
	autocrlf = true(仅本地转换为crlf)|input(提交时转换为lf)|false(不转换)

win10记事本、goland、vscode都可以自动识别换行符。任务栏会显示换行符类型。
```

4) error: path 'xxx' is unmerged when checkout

```
git rm xxx // 从git删除
```

# 5. 配置

 https://git-scm.com/book/zh/v2 

## 5.1 保存密码

```
https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%87%AD%E8%AF%81%E5%AD%98%E5%82%A8

1. 使用全局配置
git config --global credential.helper store
vim ~/.git-credentials
输入明文鉴权信息。格式(每行一个):
https://<username>:<password>@<gitdomain>

一般git都支持创建token（Personal access tokens），并指定权限。可以为服务器签出代码配置一个只读token账户。注意：gitlab使用的是token名+token验证，bitbucket使用原始用户名+token验证
bitbucket PAT创建流程：
  假设用户名为： myuser
  account-管理账户-Personal access tokens， create a token
  token name: mytoken
  permissions: read
  点创建会生成token串，记下来。这个后面无法查看。只能删掉重建。假设token为： aabbccdd
  访问时使用http访问，例如：
  http://myuser:aabbccdd@git.xxx.com/scm/xxx/xx.git
  如果是gitlab，则是类似
  http://mytoken:aabbccdd@git.xxx.com/xxx/xx.git

2. 使用当前配置（只针对当前目录，配置保存在 ./.git/config里面，似乎有问题）
git config credential.helper store
之后,密码会明文保存在 ~/.git-credentials 里面
格式(每行一个):
https://<username>:<password>@<gitdomain>
```

## 5.2 代理

### 临时使用代理

```
ALL_PROXY=https://xxx:xxx git clone https://xxx.git
ALL_PROXY=socks://xxx:xxx git clone https://xxx.git
```

## 代理配置

```
// 配置代理
git config --global http.proxy http://proxy.com:1234
git config --global https.proxy http://proxy.com:1234
git config --global http.sslverify false // 可选配置
// 清除代理
git config --global --unset http.proxy
git config --global --unset https.proxy
```

# 6. 子模块

```
https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%AD%90%E6%A8%A1%E5%9D%97
!!!第一步一定是git submodule init，否则会出莫名其妙的错误

没有git submodule init,直接git submodule add，当前repo正常，push到远端，再次checkout之后, git submodule update --init --checkout会没有反应，并且不报错，不签出子模块。

git clone --recurse-submodules https://github.com/chaconinc/MainProject
git submodule init
git submodule update
git submodule update --init --recursive
git submodule update --remote // 同步远端更改
```



