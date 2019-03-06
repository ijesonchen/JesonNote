---
title: ubuntu linux note
date: 2019-01-21 00:00:00
tags: [ubuntu, linux, note]
---

**Ubuntu linux笔记**

win10下使用ubuntu虚拟机的一些笔记

<!--more-->

[TOC]

# 环境

```
物理机：DELL latitude 7390, i5-8250U, 8G, 256G ssd
虚拟机：vmware workstation pro 15.0
发行版版本: 
ubuntu 18.04.1 (2019.02) ubuntu-18.04.1.0-live-server-amd64.iso
ubuntu 18.04.1 (2019.02) ubuntu-18.04.1-desktop-amd64.iso
ubuntu 16.04.5 (2018.07) ubuntu-16.04.5-server-amd64.iso
centos 7.6.1810.w.e17    CentOS-7-x86_64-Minimal-1810.iso
配置：四核，1G,20G，NAT。
```

# 安装

```
源选择 http://mirrors.aliyun.com/ubuntu
未使用lvm
从github下载sshkey，方便远程
****************
由于ubuntu 1804 server认为安装的目的是cloud-ready，导致虚拟机安装产生一些问题。重新安装1604版本。
desktop版本是1804，server版本是1604。
****************
如果安装desktop版本，建议选择英文。否则命令行界面容易乱码
```

# ubuntu问题

## 1. DHCP获取ip地址时，ip异常（保持不变）

```
**现象（使用dhcp获取ip地址）
1. 完整clone时，所有实例ip地址不变
2. 全新安装后，重启ip地址标保持不变，即使使用dhclient
**处理
1. 怀疑vmare NAT dhcp服务问题。换用外部dhcp，仍然存在。
2. 怀疑cloud init问题。
https://nucco.org/2018/05/ubuntu-18-04-chronicles-removing-cloud-init.html
卸载cloud-init并移除网络依赖，仍然有问题
3. 怀疑有网络相关问题。禁用外部网络，使用vmware dhcp服务。
  a. ip地址仍然不变，clone出来的实例会有冲突
  b. sudo到提示输入密码间隔10s以上
  c. 不使用网络全新安装，无法进行。
**最终方案
未解决。安装16.04。需要手动安装vim, openssh-server
```

## 2. 无法使用hostname ping通

```
**问题：未安装/启用wins服务
**解决：安装winbind（或者samba，会同时安装smbd,nmbd,winbindd）并启用wins解析
https://blog.csdn.net/NULL_LLUN/article/details/77183390
1. 安装服务
apt install winbind
apt install libnss-winbind libpam-winbind
2. 配置
vim /etc/nsswitch.conf // hosts行后加上wins
  hosts: files dns wins
  
这个时候windows应该已经能够通过hostname ping通了。
service networking/smbd/nmbd restart之后，linux机器也应该可以互相ping通了。还不行，重启机器试下。
```

## 3. 重新安装vmware tools灰色

```
**现象 vmware菜单，虚拟机，重新安装vmware tools显示灰色
**原因 没有可用光驱
**解决 给虚拟机添加一个光驱，并且设置为自动检测即可
**注意 ubuntu1804自动安装之后，系统会安装open-vm-tools包。建议不要安装vmware tools。
```

## 4. 无法支持16:9分辨率(1920x1080)

```
**问题 只有4:3或16:10分辨率，没有16:9分辨率
**失败的方案
1. 修改/etc/default/grub 无效
	GRUB_GFXMODE=1920x1080
2. 重装vmware tools。无效

**解决
https://blog.csdn.net/u013122625/article/details/52967831
命令行下
sudo su
cvt 1920 1080
  显示 Modeline "1920x1080_60.00"  173.00  1920 2048 2248 2576  1080 1083 1088 1120 -hsync +vsync
xrandr --newmode "1920x1080_60.00"  173.00  1920 2048 2248 2576  1080 1083 1088 1120 -hsync +vsync
xrandr --addmode Virtual1 "1920x1080_60.00"

vim /etc/profile // 保存对应分辨率
  末尾追加上述最后两个命令(xrandr设置)
  
修改分辨率，现在应该可以看到1920*1080了。应用即可。
```

## 5. 支持mstsc远程桌面

```
1.直接安装新的桌面环境用于远程：https://blog.csdn.net/woodcorpse/article/details/80503232
2.有桌面环境时使用mstsc连接：https://zhuanlan.zhihu.com/p/40937988
3.上述二者的总结 https://blog.csdn.net/clksjx/article/details/83445127

环境vmware+ubuntu 1804 桌面
**方法1
1. sudo apt install xrdp
2. 关闭用户自动登录
3. 开启桌面共享
4. 重启，不要登录
5. windows下使用mstsc远程桌面连接（使用ip），出现xrdp登录界面。session选Xorg，输入用户名密码登录。
   界面比较简洁。点击左上角activities就可以出现菜单。
   **注意：出现Authentication is required to create a color managed device的时候，不要点授权，直接取消。否则会出现两次授权界面后，远程桌面闪退。未排查到原因
   **注意：当远程登录时，虚拟机无法登录。
**方法2
基本按照所述操作即可。但是远程的时候登录的是新安装的xfce桌面。
但是两个桌面（本地和远程）会互相影响（比如在。

**部分程序路径
vscode: /usr/bin/code
terminal: /usr/bin/gnome-terminal
system-monitor: /snap/bin/gnome-system-monitor
```

## 6. cp: Text file busy

```
cannot create regular file 'xxx': Text file busy
如果目标文件正在执行，那么直接覆盖会提示错误
但是可以直接删除目标文件，再复制即可
```

## 7. 脚本中切换目录无效

```
https://blog.csdn.net/GX_1_11_real/article/details/80990250
脚本中有cd命令切换目录，但是退出后仍然在原来执行的目录中。即脚本中切换目录不影响当前目录
原因：shell执行脚本时是通过子shell实现的。
解决：强制在当前shell进程执行脚本
1. 使用souce命令：  source xxx.sh
2. 使用简化的.命令： . xxx.sh
```

## 8. 执行当前目录中的程序时取消取消添加./

```
出于安全考虑，执行当前目录程序时需要前缀./
可以在PATH变量中，添加一个像 ./ ，则可以直接输入程序名而不需要添加 ./
PATH=%PATH%:./
可以结合第7条通过.来在当前shell执行脚本：
. <script>
```

## 9. recovery mode：无法使用sudo

```
**背景
修改sudoer文件出错，导致无法使用sudo
1. 重启，按住shift键不放
2. grub菜单，选择Advanced options for Ubuntu, Ubuntu xxx (recovery mode)，等待启动
3. recovery menu选择 root Drop to root shell prompt。
4. root身份登录后，是只读模式，会提示Read-only file system
5. 以读写模式重新加载根目录:mount -o remount,rw / 
6. 删除错误的文件 rm etc/sudoers.d/sudoer
7. 重启 reboot

建议如果有ssh服务时，配置root远程key登录，方便恢复系统。
```

## 10. ubuntu访问windows共享（读写）

```
**环境
ubuntu 1804.5 desktop
win10 1809
**步骤
0. windows开启共享，打开读写权限。确保linux和windows互通
下述操作是在linux,root权限下
1. apt install smbclient,cifs-utils
2. 测试加载
id <linuxuser> // 查看用户uid和gid
mkdir -p /media/dev
mount -t cifs -o username=<winusername>,password="<winpassword>",uid=1000,gid=1000 //192.168.80.1/dev /media/dev
  注意：如果参数中不指定uid和gid，则只有root可以写
3. 设置自动加载
vim /etc/sambapasswd // samba授权文件，添加用户名密码
cp /etc/fstab /etc/fstab.bak // 备份
vim /etc/fstab // 添加挂载点
mount -a // 重新挂载
```

/etc/sambapasswd

```
username=<winusername>
password=<winpassword>
domain=<win domain or workgroup>
```

/etc/fstab

```
# <file system> <mount point>   <type>  <options>       <dump>  <pass>
//192.168.80.1/dev /media/dev cifs rw,user,uid=1000,gid=1000,credentials=/etc/sambapasswd 0 0
```



# centos问题

## 1. 启用网卡

```
1. ip addr 获取网卡名称（如ens33)
2. root用户: 
   vi /etc/sysconfig/network-scripts/ifcfg-ens33 (记得备份)
     修改ONBOOT=no -> yes
3. 重启服务：service network restart
```



# 知识库

## 1. 常见信号
	参见go-note.md

## 2. 僵尸进城defunct

```
https://www.jb51.net/LINUXjishu/457748.html
**产生
1. 进程已经结束
2. 父进程不是init
   进程结束时，linux会扫描其子进程，并通过init来接管，从而保证每个进程都有一个父进程。
3. 父进程没有等待它

**Linux进程运作方式
1. fork()时分配新的entry
2. 退出时执行exit()调用保存残留数据（主要是一些统计信息）
3. 残留数据传递给父进程
4. 释放残留数据

**危害
占用进程表项，消耗pid，可能导致资源耗尽

**处理
1. kill -9 // 多数情况下无效
2. 杀死其父进程 // 让init接管后处理
2. 重启系统
```



# 常见操作

## 0. 常用命令

```
sudo xxx // 以root权限运行xxx命令
sudo su // 切换到root用户。在root用户下运行passwd设置密码，即可以root直接登陆
su <username> // root用户，切换到用户<username>
passwd // 修改密码。可指定用户
apt update|install|remove // 更新源|安装软件|移除软件
uname -a // 显示kernal信息
lsb_release -a // 显示发行版本信息
rpm -q centos-release // cenos发行版本信息
who // 查看登录用户
whoami // 查看当前登录用户
scp -i <ssh-key> -r src dst 
	// 跨服务器复制文件 -r 复制目录。路径格式user@addr:<path>,可使用 ~ 代表home
chmod -R o+u <dir> // 递归地对<dir>目录及子目录下文件添加其他用户的写权限(ugo)
mkdir -p dir1/dir2/dir3 // 递归创建多级目录
kill -SIGKILL `pidof xxx` // 强制杀死xxx, sig=9
pkill -SIGTERM xxx // 通知xxx退出,sig=15
<userbin> <params> >> log.txt 2>&1 & 
	// 启动程序，并将屏幕输出到log.txt，stderr从定向到stdout，即将stdout和stderr输出到log.txt
id // 查看当前用户的uid和gid
```

### 0.1 grep

```
grep [options pattern-spec [files...]
-i: 模式匹配时忽略大小写
-V: 显示不匹配的行
-l: 列出匹配模式的文件名称，而不是打印匹配的行
-n: 列出检索目标所在的行号
-c: 统计匹配的行总数，不显示航信息

```



## 1. 知识库

```
常见信号：
	参见go-note.md
```



##  2. 修改主机名

```
sudo su
vim /etc/cloud/cloud.cfg
	preserve_hostname: false -> true
vim /etc/hostname
vim /etc/hosts // 修改127.0.0.1的映射。会影响sudo等命令的执行速度
```

## 3. 修改密码

```
passwd 修改当前用户密码，有长度限制
sudo passwd username 修改用户密码，可以设置为简单密码
```

## 4. 更换阿里源

ubuntu

```
sudo su
cd /etc/apt
cp source.list source.list.bak // 备份
vi source.list
  a. 注释掉 deb cdrom: 开头的这一行（光盘安装路径）
  b. 替换为mirrors.aliyun.com:
	:%s/us.archive.ubuntu/mirrors.aliyun/
	:%s/security.ubuntu/mirrrors.aliyun/
  c. 保存退出
  	:wq
apt update
```

centos

```
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
curl -o /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo

https://yq.aliyun.com/articles/33286
CentOS系统更换软件安装源
第一步：备份你的原镜像文件，以免出错后可以恢复。
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup
第二步：下载新的CentOS-Base.repo 到/etc/yum.repos.d/
CentOS x
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-x.repo

更改CentOS-Media.repo使其为不生效：
enabled=0

第三步：运行yum makecache生成缓存
yum clean all
yum makecache
```



# 安装

## 1. vscode

```
https://blog.csdn.net/HelloZEX/article/details/80763558
1. 下载
https://code.visualstudio.com/Download
https://vscode.cdn.azure.cn/stable/1b8e8302e405050205e69b59abb3559592bb9e60/code_1.31.1-1549938243_amd64.deb
2. 安装
sudo dpkg -i xxx.deb

golang配置，windows下golang tools迁移到linux，参考go-note.md
```

## 2.  ssh

```
服务器：
sudo su
apt install openssh-server // 安装服务
ip addr // 记录ip地址

客户端:
ssh-keygen // 生成本机密钥。如果已有则跳过
 // 复制本机用户公钥到服务器，需要输入密码，同时要求服务器允许user使用密码登录。
 // 如果远程用户名相同可忽略user。
 // -i指定公钥文件，不指定则使用当前用户的公钥（一般存放在~/.ssh/id_rsa.pub）
 // -i指定的文件如果不是pub结尾，则自动使用对应的xxx.pub文件，找不到提示错误
 // -i制定的pub文件可以有多个key，都会被添加
 // -f如果没有对应的私钥文件user_id_rsa，则必须指定-f（用于为其他用户更新key且禁用了密码登录时）
ssh user@ipaddr // 登录到服务器

提升服务器安全性：
sudo vim /etc/ssh/sshd_config // 编辑配置文件。建议先备份。
  PermitRootLogin yes|no|prohibit-password  // root登录：允许|禁止|禁用密码(仅key)。
  			// 建议prohibit-password并且添加key到root，方便恢复系统。
  PasswordAuthentication yes|no // 使用密码登录：允许|禁止。建议no
service ssh restart // 重启服务
```



## 3. zsh (一个好用的shell)

安装

```
sudo apt install zsh
** oh-my-zsh是一个zsh的主题，包括一些git相关的alias，很方便 
** 需要先安装git
sudo sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"

** 查看当前shell
echo $SHELL // 查看配置的shell
echo $0		// 当前使用的shell

** 修改默认shell
chsh

** 桌面版的terminal修改默认shell后，需要重启后才会生效
```
修改提示符信息
```
https://www.jianshu.com/p/bf488bf22cba
安装oh-my-zsh后，启动脚本.zshrc：1)设置ZSH_THEME 2)update oh-my-zsh 3)应用主题。默认是
➜ <dir> <git-info>
其中，第一个箭头是根据命令执行是否成功变换颜色（由主题中 {ret_status} 控制）
自定义方式
1).oh-my-zsh/themes中新建主题。可以由其他主题修改而来。
2)修改.zshrc中ZSH_THEME指向新主题 
3)source .zshrc生效。如果已经设置好ZSH_THEME，则修改主题可直接 source .oh-my-zsh/oh-my-zsh.sh 应用主题
字段设置一般是 %{$fg[cyan]%}%c%{$reset_color%} 。其中第一部分fg指定前景色，中间%c表示目录名。常见的%n 用户名，%m 机器名，%M 机器全名等。
```
效果
```
(提示较长，命令输入是在第二行开始)
➜  user@host <fullpath> git:(branch_name) ✗
$ 
```
示例
```
local ret_status="%(?:%{$fg_bold[green]%}➜ :%{$fg_bold[red]%}➜ )"
PROMPT='${ret_status} %{$reset_color$fg[cyan]%}%n@%M%{$reset_color%} %{$fg_bold[green]%}%d%{$reset_color%} $(git_prompt_info)
$ '

ZSH_THEME_GIT_PROMPT_PREFIX="%{$fg[cyan]%}git:(%{$fg_bold[red]%}"
ZSH_THEME_GIT_PROMPT_SUFFIX="%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_DIRTY="%{$reset_color$fg[cyan]%}) %{$fg_bold[yellow]%}✗"
ZSH_THEME_GIT_PROMPT_CLEAN="%{$reset_clolr$fg[cyan]%})"
```

## 4. supervisor

```
系统 ubuntu 18.04.1，全程需要root权限（sudo su）
http://supervisord.org/installing.html
**安装
1. apt install supervisor // 建议，会自动配置service，可能需要调整参数
2. python安装（可以指定版本）：
   a)apt install python
   b)python pip install
   c)pip install supervisor==3.1.4 // 指定版本安装
3. supervisor配置
4. service配置
5. 启动
   1) supervisord -c <configfile> // 可以直接看到启动是否正常
   2) service supervisor start|stop|restart // 使用服务
      service supervisor status // 查看状态，包括启动日志
```

supervisor配置
```
1.建立supervisor目录并进入，例如
  mkdir /etc/supervisord.d
  cd /etc/supervisord.d
2.生成配置文件(echo_supervisord_conf是一个脚本)
  echo_supervisord_conf > supervisord.conf
3.编辑supervisord.conf（配置文件里都有对应字段，可能被注释掉了）

[inet_http_server]
port=127.0.0.1:9001

[supervisorctl]
serverurl=http://127.0.0.1:9001 

[include]
files = /etc/supervisord.d/*.ini
```

service配置

```
ubuntu 1804 service配置文件位置
/lib/systemd/system/supervisor.service
（老系统在/etc/systemd/system目录）
注意修改Service部分参数

[Unit]
Description=Supervisor process control system for UNIX
Documentation=http://supervisord.org
After=network.target

[Service]
ExecStart=/usr/bin/supervisord -n -c /etc/supervisord.d/supervisord.conf
ExecStop=/usr/bin/supervisorctl $OPTIONS shutdown
ExecReload=/usr/bin/supervisorctl -c /etc/supervisord.d/supervisord.conf $OPTIONS reload
KillMode=process
Restart=on-failure
RestartSec=50s

[Install]
WantedBy=multi-user.target
```



# SHELL脚本

## 1. 规范

### 1.1基本语法

```
#!/bin/bash
“#!”指定解释器。不要加空格。

作为可执行程序，或作为shell程序参数
./test.sh
/bin/sh test.sh

参数：$n 表示第n个参数，0是命令本身。10以上要加{}

变量：变量名不加美元符号，变量名和等号之间不能有空格。可以冲定义
your_name="qinjx" // 显式赋值
for file in `ls /etc` // 语句赋值
使用：变量名前面加美元符号，花括号是可选的，建议加上
echo ${your_name} 
echo ${your_name}xxx

环境变量
export var=val 		#设置值
readonly var[=val]	#设置var为只读
unset var			#清除变量

注释：开头加#
大段注释：写成函数，不调用即可

字符串：也可以不加引号
单引号：原样输出，不能出现单引号
双引号：可以使用变量，不能出现双引号
拼接：直接挨着写："hello, "$your_name" !"
求长度：${#string}
子串（0基，闭区间）：${string:1:4}
查找：echo `expr index "$string" abcd` # 查找abcd中任意一个在string中第一次出现的位置
其他：http://tldp.org/LDP/abs/html/string-manipulation.html
```
流程控制及条件判断
```
**条件判断condition
[ 其实是一个程序（ /usr/bin/[ ，有时是/usr/bin/test的软链接)。因此使用时后面要加空格，以]表示输入结束，结束时设置返回值为表达式的值。详情可以参考 man [。例如(注意空格！)：
[ $foo = "bar" ]
格式：
[ EXPRESSION ]	# 常用
[ ]				# 默认为false
[ OPTION		# 如help或version
一元操作符
() , ! # 表达式
-n , -z # 字符串（非空/空字串）。注意数值不是空字符串。
二元操作符
= , !=  # 字符串
-a (and), -o (or) # 表达式
-eq , -ne , -ge , -gt , -le , -lt  # 数值
（经测试，= !=可以用在数值上，-eq -ne也可以用在字符串上。似乎shell会在数值和字串之间简单转换？）
另外，还有判断文件的一些操作

if condition
then
  cmd1
  ...
elif condition2
  cmd1
  ...
else
  cmd1
  ...
fi

for var in item1,....
do
  cmd1
  ...
done
 
while cond
do
  cmd
  ...
done

无限循环:
while true
while :
for (( ; ; ))
 
until cond
do 
  cmd
done

case ${var} in
    val1)
        #cmd1
   	 	;;
    ...
    *） 
    	#default
    	;;
esac
```
其它
```
##printf：格式化 format可以不加引号
printf “%d,%s\n” 1 abc

##tr：转换字符串 -s 去重 -d删除
tr [options] cource-char-file replace-char-list
tr "[a-z]" "[A-Z]" 	#小写到大写
tr -s ["\n"] 		#去重重复换行，即删除空行
tr -d ‘\r’			#删除回车

##运行字符串
eval ${stringval}

##包含其他文件
. other.sh
获取文件输入的绝对路径
real_path=`readlink -f ${other.h}` 
. $real_path

##特殊文件
/dev/null 	#输出：丢弃。输入：结束
/dev/tty 	#终端

##函数
function func_name(){
    xxx
    return xxx | echo xxx
}
函数返回值：
return: 设置 $?。调用后立即使$?获取。但是，这个变量会被随后的调用覆盖
echo: 使用 var=`func`方式获取
```

 ### 1.2 特殊字符和正则

特殊字符

```
#: 注释字符
\: 将特殊字符或者通配符还原成一般字符
|: 管道符，分割两个管线命令的界定
;: 连续命令下的分隔符
~: 用户的home目录
$: 放在变量前面，正确使用变量
&: 工作控制，将命令编程背景下工作
!: 非 (!) 的意思，逻辑运算符
>,>>: 输出重定向，分别是覆盖和追加
<,<<: 输入重定向
'': 单引号，不具有变量置换的功能
"": 双引号，具有变量置换的功能
(): 在中间的为子 shell 的起始与结束
{]: 在中间为命令块的组合
```

正则字符

```
^: 匹配行首位置
$: 匹配行尾位置
.: 匹配任意祖父
: 对 之前的匹配整体或字符匹配任意次 (包括 0 次)
\?: 对 \? 之前的匹配整体或字符匹配 0 次或 1 次
{n}: 对 \ { 之前的匹配整体或字符匹配 n 次
{m,}: 对 \ { 之前的匹配整体或字符匹配至少 m 次
{m,n}: 对 \ { 之前的匹配整体或字符匹配 m 到 n 次
[abcdef]: 对单字符而言匹配 [] 中的字符
[a-z]; 对单字符而言，匹配任意一个小写字母
```

 ## 1.3 其他

```
**复杂子串拼接
一对引号中不允许出现单引号，即使转义也不行
解决：拼接。例子

**调试
bash -x shell param

**自增
例子
```



