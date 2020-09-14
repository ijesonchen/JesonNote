---
title: ubuntu linux note
date: 2019-01-21 00:00:00 +0800
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
4. 指定uid和gid后，对应用户也可以直接使用mount|umount <mountpoint>加载/卸载
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

## 3. 常见目录和变量

```
/boot  	// 启动
/dev	// 设备
/etc	
/home	// 用户home
/mnt	// 加载
/opt	
/proc	// 进程信息,每id一个目录
/root	// root的home
/usr
/var

/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/bin:/usr/local/sbin

常见变量
PATH
HOME // 可以直接修改当前home设置
SHELL // 当前shell
PWD // 当前目录
LANG LANGUAGE LC_ALL LC_CTYPE LOCALE// 语言相关. C.UTF-8
HOSTTYPE=x86_64
LOGNAME // 登录用户名
```



# 常见操作

## 0. 常用命令

```
sudo xxx // 以root权限运行xxx命令
sudo su // 切换到root用户。在root用户下运行passwd设置密码，即可以root直接登陆
su <username> // root用户，切换到用户<username>
passwd // 修改密码。可指定用户
apt update|install|remove // 更新源|安装软件|移除软件。
	E: Unable to fetch some archives, maybe run apt-get update or try with --fix-missing?
	如果安装失败，首先考虑update后再次安装
who // 查看登录用户
whoami // 查看当前登录用户
scp -i <ssh-key> -r src dst 
	// 跨服务器复制文件 -r 复制目录。路径格式user@addr:<path>,可使用 ~ 代表home
chmod -R o+u <dir> // 递归地对<dir>目录及子目录下文件添加其他用户的写权限(ugo)
mkdir -p dir1/dir2/dir3 // 递归创建多级目录
kill -SIGKILL `pidof xxx` // 强制杀死xxx, sig=9
pkill -SIGTERM xxx // 通知xxx退出,sig=15
<userbin> <params> >> log.txt 2>&1 & 
	// 启动程序，并将屏幕输出到log.txt，stderr重定向到stdout，即将stdout和stderr输出到log.txt
	// 可以使用nohup 
id // 查看当前用户的uid和gid
sz rz // 通过终端发送（send）接收（recv）文件
ps aux -q 22278 --sort -rss // 显示进程信息，按照rss常驻内存集排序, pmem内存占用率
watch -n 0.5 'cmd param' // 每隔0.5秒执行命令并显示结果
cut -c 1-10,20-30 // 显示一行的1-10,20-30的字符。可配合grep使用
ping // 统计中mdev是rtt的标准差standard deviation
dmesg // 查看系统消息， oom-killer 表示内存占用过多被杀掉
转换dmsg时间到可读时间
dmsg | tail | sed -r 's#^\[([0-9]+\.[0-9]+)\](.*)#echo -n "[";echo -n $(date --date="@$(echo "$(grep btime /proc/stat|cut -d " " -f 2)+\1" | bc)" +"%c");echo -n "]";echo -n "\2"#e' 

pstree -p | wc -l // 查询整个系统已用的线程或进程数
ulimit -a // 查询当前用户限制(文件句柄,最大线程数等)

fdisk -l // 列出磁盘信息
mount <disk> <target_dir> -r // 挂载磁盘, 只读方式, 默认-w读写
umask u=, g=w, o=rwx // 利用umask命令可以指定哪些权限将在新文件的默认权限中被删除。使得组用户的写权限，其他用户的读、写和执行权限都被取消：
```

### 0.0 查看发行版

```shell
uname -a // 显示kernal信息
lsb_release -a // 显示发行版本信息
rpm -q centos-release // cenos发行版本信息
版本信息文件(较新版本支持)
ubuntu: /etc/os-release
centos: /etc/system-relase
```



### 0.1 grep

```
grep [options pattern-spec [files...]
-i: 模式匹配时忽略大小写
-V: 显示不匹配的行
-l: 列出匹配模式的文件名称，而不是打印匹配的行
-n: 列出检索目标所在的行号
-c: 统计匹配的行总数，不显示航信息

多个关键字 "kwd1\|kwd2"
```

tail + grep + cut不输出的问题

```
https://stackoverflow.com/questions/14360640/tail-f-into-grep-into-cut-not-working-properly
the problem is almost certainly related to how grep and cut buffer their output. here's a hack that should get you around the problem, though i'm sure there are prettier ways to do it:

tail -f /var/somelog | while read line; do echo "$line" | grep "some test and p l a c e h o l d e r" | cut -f 3,4,14 -d " "; done
tailf engine_error.log| while read line; do echo $line|grep test_flag|cut -c 120-; done
```



### 0.2 vim

```
模式：一般模式，编辑模式，命令模式.
	其他模式由一般模式进入，按esc返回一般模式。一般模式下按v进入visual模式，可以实现文本选择。
	一般->编辑：i，I,a,A,o,O
	一般->命令：: 或者 /
	一般->可视：v
**一般模式
复制行：yy
复制n行：nyy
复制：v（visual）,方向键选择文本，y（复制）
黏贴：p
撤销：uu
重做：^r

## 命令
:s 替换文本
	:s/src/dst/g   当前行所有的src替换为dst（没有最后的g表示替换第一个）
	:%s/src/dst/g  所有行所有的src替换为dst

配置文件
/etc/vim/vimrc  /etc/vimrc 全局	
~/.vimrc 用户
配置
set mouse-=a " 进制vim内置的鼠标支持。可以恢复鼠标本来的用途（如右键黏贴等）
set nu " 显示行号
set hlsearch " 搜索高亮
syntax on " 语法高亮
set showcmd " 命令模式下，在底部显示键入的命令
set t_Co=256 " 256色支持
set autoindent " 自动缩进
set ruler " 行列号
set showmatch " 匹配括号
```

### 0.3 tar

```
tar -zcvf file.gz dir // 将dir压缩至file.tar
tar -C dir -xzf file.gz // 将file.gz解压到dir
参数
C change to dir 
c create
x extract
f file
z gzip
v verbose
d diff
r append
t list
u update
j bzip2
```

### 0.4 awk

#### 基本概念

https://www.cnblogs.com/chengmo/archive/2010/10/06/1844818.html

内置变量

| $0          | 当前记录（作为单个变量）            |
| ----------- | ----------------------------------- |
| $1~$n       | 当前记录的第n个字段，字段间由FS分隔 |
| FS          | 输入字段分隔符 默认是空格           |
| NF          | 当前记录中的字段个数，就是有多少列  |
| NR          | 已经读出的记录数，就是行号，从1开始 |
| RS          | 输入的记录他隔符默 认为换行符       |
| OFS         | 输出字段分隔符 默认也是空格         |
| ORS         | 输出的记录分隔符，默认为换行符      |
| ARGC        | 命令行参数个数                      |
| ARGV        | 命令行参数数组                      |
| FILENAME    | 当前输入文件的名字                  |
| IGNORECASE  | 如果为真，则进行忽略大小写的匹配    |
| ARGIND      | 当前被处理文件的ARGV标志符          |
| CONVFMT     | 数字转换格式 %.6g                   |
| ENVIRON     | UNIX环境变量                        |
| ERRNO       | UNIX系统错误消息                    |
| FIELDWIDTHS | 输入字段宽度的空白分隔字符串        |
| FNR         | 当前记录数                          |
| OFMT        | 数字的输出格式 %.6g                 |
| RSTART      | 被匹配函数匹配的字符串首            |
| RLENGTH     | 被匹配函数匹配的字符串长度          |
| SUBSEP      | \034                                |



#### 统计平均值

```
示例文本
gc 1051 @5853.090s 0%: 0.44+74+0.52 ms clock, 14+619/553/2.6+16 ms cpu, 38960->39111->19985 MB, 39959 MB goal, 32 P
// 统计平均耗时平均值 0.44+74+0.52 中的74
grep gc a.log| awk '{split($5,array,"+");print array[2]}'| awk '{sum+=$1} END {print ":", sum/NR}'
// 统计平均值，和 耗时>5 的平均值
grep gc a.log| awk '{split($5,array,"+");print array[2]}'| awk '{sum1+=$1;if($1>5){sum2+=$1;cnt+=1}} END {print ":",sum1/NR,sum2/cnt}'

// 关于END语法
awk '{ // each record state } END { // end state }'

流程解释
grep输出后，第一个awk按照空格分隔为多个参数。取第5个（1基）"0.44+74+0.52"，按照+分隔为数组后取第二个"74"
第二个awk处理数据。END输出结果。NR表示处理的行数。多个语句使用分号 ; 分隔。参数和if条件使用()。

比较：会自动转换两个变量，如果不能按数值比较，则按字符串比较。如果强制按数值比较，可以使用+0
if ($var > 10) // 如果var不能转换为数值，则按照 "var" 和 "10" 比较
fi ($var+0 > 10) // 强制按数值。var无法转换为时是0
```

#### 获取header及值

```
awk -F'\t' '{print $1,$19,$36,$37}' newcontent.data|head -1  // 确认header名字
head newcontent.data -n 10|awk -F'\t' '{print $1,$19,$36,$37}' // 打印对应值
```





### 0.5 crontab

定时运行程序

```
crontab使用入门
https://www.cnblogs.com/bourneli/archive/2012/04/14/2446944.html 
参数
-e edit编辑配置,退出时会检查配置
-l list
-r remove

语法（用户级）cmd建议使用绝对路径。
# min hour day(1-31) month(1-12) week(0-7) cmd
* 所有
, 多个
- 区间
*/n 间隔n

参考
https://crontab.guru/examples.html

示例
59 23 11 27 * mail benben < /home/dmtsai/lover.txt // 每一年11月27日23分59 秒发一封情书给benben
0 17 * * 5 mail all_members < weekily_report_notify // 每周五5点整，提醒所有组员发周报
0 23 28-31 * * [ "$(/bin/date +\%d -d tomorrow)" = "01" ] && /your/script.sh // 每月最后一天 23点。使用每月第一天更简单，效果类似。
* */3 * * * test // 每分钟执行一次，每3小时执行一次。会在3的整数倍小时的每分钟执行
0 */3 * * * test // 每3小时的0分执行
*/180 * * * * test // 每小时0分执行。除以60以上，都只会在每小时的0分执行。
*/180 */2 * * * test // 每双数小时的0分执行。除以60以上，都只会在每小时的0分执行。
0 * * * * test // 每到0分执行

系统级的crontab只有root权限有权编辑，该crontab是一个文件，位置为/etc/crontab，需要在频率和命令之间添加命令执行者，并且可以添加一些全局变量，在调度中使用：
MAILTO=root
/15 * * * * root test 
```

### 0.6 iptables

```
Linux下iptables 禁止端口和开放端口
https://975156298.iteye.com/blog/2323688 
root账户下
一。端口的开启。
     iptables -A INPUT -p tcp --dport 80 -j ACCEPT
     iptables -A OUTPUT -p tcp --sport 80 -j ACCEPT
     -A：参数就看成是添加一条 INPUT（OUTPUT） 的规则；
     -p：指定是什么协议 我们常用的tcp（udp）协议。 例如：53端口的DNS到时我们要配置DNS用到53端口，大家就会发现使用udp协议的；
    --dport：就是目标端口，当数据从外部进入服务器为目标端口；
    --sport：数据从服务器出去 则为数据源端口；
  	-j 就是指定是 ACCEPT 接收 或者 DROP 不接收。
 上面这两行代码是开启 80 端口。

二。端口的关闭。
    iptables -A INPUT -p tcp --dport 80 -j DROP
    iptables -A OUTPUT -p tcp --sport 80 -j DROP
这样就关闭了 80 端口。

三。查看端口命令。
    iptables -L -n    用来查看 iptables 规则。也就是开启的端口等。

四。删除 iptables 规则。
   iptables -L -n --line-numbers   查看规则 前面加上序号。
   iptables -D INPUT [ 序号]  通过上面查看得到的序号，通过序号进行删除 iptables 规则。

五。规则的保存。
     配置完成后是需要保存的。这时就需要 root 权限。sudo 权限已经不够了。设置好后。输入代码：
     iptables-save > /etc/iptables-rules
     ip6tables-save > /etc/ip6tables-rules
    我们需要编辑 /etc/network/interfaces 文件，在最后插入下面两行：
    pre-up iptables-restore < /etc/iptables-rules
    pre-up ip6tables-restore < /etc/ip6tables-rules
重启电脑后。使用   sudo iptables -L     查看配置是否生效。

六。转发流量
1. 修改/etc/sysctl.conf 然后找到 net.ipv4.ip_forward = 0 修改为 net.ipv4.ip_forward = 1 随后保存。
2. 端口转发
iptables -t nat -A PREROUTING -p tcp --dport [要转发的端口号] -j DNAT --to-destination [要转发的服务器IP]
端口段标识 a:b 标识从a-b范围的端口
4. 保存并重启iptables
service iptables save
service iptables restart

常用
iptables -D OUTPUT 10 // 删除第10个
iptables -A OUTPUT -p tcp -d localhost -j ACCEPT
iptables -A OUTPUT -p tcp -d 127.0.0.1 -j ACCEPT
iptables -A  OUTPUT -p tcp -d 172.16.59.192 -j ACCEPT
iptables -A OUTPUT -p tcp --dport 3061 -j DROP
iptables -A OUTPUT -p tcp --dport 2051 -j DROP
iptables -L -n --line-numbers
```

### 0.7 top

```
https://www.cnblogs.com/ggjucheng/archive/2012/01/08/2316399.html
top   //每隔5秒显式所有进程的资源占用情况
top -d 2  //每隔2秒显式所有进程的资源占用情况
top -c  //每隔5秒显式进程的资源占用情况，并显示进程的命令行参数(默认只有进程名)
top -p 12345 -p 6789//每隔5秒显示pid是12345和pid是6789的两个进程的资源占用情况
top -d 2 -c -p 123456 //每隔2秒显示pid是12345的进程的资源使用情况，并显式该进程启动的命令行参数
```

#### 替代工具

```
1. htop
yum install htop
显示每个cpu/mem负载

2. vtop 基于nodejs
npm install -g vtop
字符界面模拟windows任务管理器

3. gtop 基于nodejs
npm install -g gtop
类似vtop，多彩显示，功能多一点

4. ptop 基于python
pip install ptop

** 安装nodejs
yum install nodejs
```

### 0.8 strings

```
显示二进制文件中可打印字符串。
可用于检索bin中的符号
strings /lib64/libc.so.6 |grep GLIBC_
显示libc中含有GLIBC_的符号，一般用于查看其中函数对应的libc版本
```

### 0.9 ssh-key

```
ssh-keygen -l -E md5 -f <keyfile>
校验key的md5签名，一般是pubkey。可用于和gitlab上的key比对。
```

### 0.10 sed

```
**替换**
https://www.cnblogs.com/linux-wangkun/p/5745584.html

sed 's/原字符串/替换字符串/'           # 替换第一个
sed "s/原字符串包含'/替换字符串包含'/"  # 要处理的字符包含单引号
sed 's?原字符串?替换字符串?'           # 将分隔符换成问号”?”
sed 's/原字符串/替换字符串/g'          # 替换所有匹配关键字
　　”^”表示行首
　　”$”符号如果在引号中表示行尾，但是在引号外却表示末行(最后一行)
# 注意这里的 " & " 符号，如果没有 “&”，就会直接将匹配到的字符串替换掉
sed 's/^/添加的头部&/g' 　　　　 #在所有行首添加
sed 's/$/&添加的尾部/g' 　　　　 #在所有行末添加
sed '2s/原字符串/替换字符串/g'　 #替换第2行
sed '$s/原字符串/替换字符串/g'   #替换最后一行
sed '2,5s/原字符串/替换字符串/g' #替换2到5行
sed '2,$s/原字符串/替换字符串/g' #替换2到最后一行
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
sed 's/archive.ubuntu.com/mirrors.aliyun.com/g' sources.list.bak > sources.list

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

## 5. ssh自动登录脚本

```

```

## 5. 调整cpu性能

```
https://www.jianshu.com/p/e1a37771c68e
CPU frequency scaling - https://wiki.archlinux.org/index.php/CPU_frequency_scaling

cpufreq的五种模式
cpufreq是一个动态调整cpu频率的模块，系统启动时生成一个文件夹/sys/devices/system/cpu/cpu0/cpufreq/，里面有几个文件，其中scaling_min_freq代表最低频率，scaling_max_freq代表最高频率，scalin_governor代表cpu频率调整模式，用它来控制CPU频率。 
performance最高 powersave最低 userspace用户定义 conservative平滑 ondemand按需跳频 
```

## 6. 临时指定环境变量

```
export GODEBUG="gctrace=1"
如果需要永久生效，编辑用户home目录下的
.rc .bashrc 或 .zshrc，取决于使用的shell

如果只针对某次命令生效，
GODEBUG="gctrace=1" go build .
LIBRARY_PATH=/usr/lib/x86_64-linux-gnu build.sh
```

## 7 curl命令行查看ip公网地址

```
curl ipinfo.io 
curl cip.cc
curl ip.gs
curl ifconfig.me
```



# 安装

## 0. 安装及位置

源码安装

```
一般步骤: 下载，解压，配置，编译
wget http://xxx/xxx.tar.gz
tar -zxvf xxx.tar.gz
cd xxx
./configure
make
然后会在当前目录生成可执行文件
```

包管理工具

```
apt/yum install xxx
apt/yum search xxx

查看安装包路径
centos:
rpm -qa |grep blas // 获取详细包名==> blas-devel-3.4.2-8.el7.x86_64
rpm -ql blas-devel-3.4.2-8.el7.x86_64
ubuntu:
dpkg -l | grep blas  // 获取详细包名==> libopenblas-dev:amd64
dpkg -L libopenblas-dev:amd64
```

### 常用包

```
yum install lrzsz  // rz sz
yum install httpd-tools // ab
yum install util-linux  // tailf
yum install sysvinit-tools // pidof
yum install iproute
yum install zsh
yum install gcc-toolset-9-gcc-c++ gcc-toolset-9-make gcc-toolset-9-gcc-gdb-plugin gcc-toolset-9-valgrind gcc-toolset-9-perftools gcc-toolset-9-binutils gcc-toolset-9-gdb-gdbserver
yum install bazel3 // 需要先配置repo源，参考官网

brew cask install iterm2 typora docker visual-studio-code google-chrom
brew install iproute2mac // ip addr
brew install md5sha1sum wget clang-format graphviz xdot

ubuntu:
build-essential // gcc开发工具集 也可指定 gcc-8 g++-8
zsh git wget curl iproute2
```



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

#### 安装

```shell
服务器：
sudo su
apt install openssh-server // 安装服务
ip addr // 记录ip地址

ssh -A host1 -A host2 // 本机->host1->host2，-A转发秘钥
ssh -p port1 user1@host1 ssh -p port2 user2@host2 // 本机->host1->host2，不过登录都是用服务器上的秘钥

客户端:
ssh-keygen // 生成本机密钥。如果已有则跳过
 // 复制本机用户公钥到服务器，需要输入密码，同时要求服务器允许user使用密码登录。
 // 如果远程用户名相同可忽略user。
 // -i指定公钥文件，不指定则使用当前用户的公钥（一般存放在~/.ssh/id_rsa.pub）
 // -i指定的文件如果不是pub结尾，则自动使用对应的xxx.pub文件，找不到提示错误
 // -i制定的pub文件可以有多个key，都会被添加
 // -f如果没有对应的私钥文件user_id_rsa，则必须指定-f（用于为其他用户更新key且禁用了密码登录时）
ssh user@ipaddr // 登录到服务器

ssh -D :9898 user@hostip -p sshport
本地启动端口9898，通过hostip代理上网（sock5）

提升服务器安全性：
sudo vim /etc/ssh/sshd_config // 编辑配置文件。建议先备份。
  PermitRootLogin yes|no|prohibit-password  // root登录：允许|禁止|禁用密码(仅key)。
  			// 建议prohibit-password并且添加key到root，方便恢复系统。
  PasswordAuthentication yes|no // 使用密码登录：允许|禁止。建议no
service ssh restart // 重启服务
```

#### 自动登录

```
xshell可以设置登录脚本
使用expect脚本实现，并且可以传递参数
https://www.jianshu.com/p/4178badeee39
http://www.cnblogs.com/zhenbianshu/p/5867440.html
```

#### 代理

```
https://zhuanlan.zhihu.com/p/57630633
ssh 命令除了登陆外还有三种代理功能：

正向代理（-L）：相当于 iptable 的 port forwarding 在本地启动端口，把本地端口数据转发到远端。

ssh -L 127.0.0.1:1111:127.0.0.1:2222 root@3.3.3.3
本地1111端口映射到3.3.3.3的本地2222端口

反向代理（-R）：相当于 frp 或者 ngrok 让远端启动端口，把远端端口数据转发到本地。
socks5 代理（-D）：相当于 ss/ssr 本地启动sock5，通过远端代理访问。
```



## 3. zsh (shell)

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

** alias命令
很多alias命令,方便操作
-='cd -'
3='cd -3'
...=../..
gcb='git checkout -b'
ggsup='git branch --set-upstream-to=origin/$(git_current_branch)'
alias // 查看所有alias命令

** 支持的函数
zsh有一些函数方便操作，可以直接在命令行使用
如 
git_prompt_long_sha //显示当前sha值等 
zsh_stats // 显示命令使用统计
print -l ${(ok)functions} // 打印所有函数
whence -f git_prompt_long_sha // 显示函数代码



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

问题排查

```
-v    equivalent to --verbose
-x    equivalent to --xtrace
可以在zhs执行命令时显示详细trace信息，帮助排查错误或性能瓶颈，例如，某些情况容易卡住
```

```
****git目录非常慢****
macos下运行docker，ubuntu，命令行操作git目录非常慢，执行完一条命令后会卡几秒
zsh -vx后，执行ls命令
发现会在
+parse_git_dirty:18> STATUS=+parse_git_dirty:18> STATUS=+parse_git_dirty:18> tail -n1
这一行停留几秒。推测是由于git命令的原因
执行git status，发现是该命令返回时间较长，经常会重建index
切换到bash下，执行git status仍然缓慢
**发现该目录是从docker访问的macos路径
讲该repo复制到docker内部存储后，问题消失
推测原因：docker存储macos文件系统时，存在性能问题，会导致访问大量小文件比较慢。
同样在windows系统下，wsl访问windows目录也容易出现类似问题。
**可以考虑直接通过vscode的Remote - Containers插件访问docker内的文件来解决
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
   附后
   修改配置后，supervisorctl update更新配置
4. service配置
   附后
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

; 可以有多个。配置多个时可能导致冲突
[include]
files = /etc/supervisord.d/*.ini


**************************示例配置*******************************
; supervisor config file

[unix_http_server]
file=/var/run/supervisor.sock   ; (the path to the socket file) UNIX socket 文件，supervisorctl 会使用
chmod=0700                       ; sockef file mode (default 0700) socket 文件的 mode，默认是 0700

[supervisord]
logfile=/var/log/supervisor/supervisord.log ; (main log file;default $CWD/supervisord.log) 日志文件，默认是 $CWD/supervisord.log
pidfile=/var/run/supervisord.pid ; (supervisord pidfile;default supervisord.pid) pid 文件
childlogdir=/var/log/supervisor            ; ('AUTO' child log dir, default $TEMP)

; the below section must remain in the config file for RPC
; (supervisorctl/web interface) to work, additional interfaces may be
; added by defining them in separate rpcinterface: sections
[rpcinterface:supervisor]
supervisor.rpcinterface_factory = supervisor.rpcinterface:make_main_rpcinterface

[supervisorctl]
serverurl=unix:///var/run/supervisor.sock ; use a unix:// URL  for a unix socket 通过 UNIX socket 连接 supervisord，路径与 unix_http_server 部分的 file 一致

; 在增添需要管理的进程的配置文件时，推荐写到 `/etc/supervisor/conf.d/` 目录下，所以 `include` 项，就需要像如下配置。
; 包含其他的配置文件
[include]
files = /etc/supervisor/conf.d/*.conf ; 引入 `/etc/supervisor/conf.d/` 下的 `.conf` 文件
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


************示例配置*****************
[program:engine]
directory=/data/app/engine/current
command=/data/app/engine/current/bin/engine_run -config=/data/app/engine/current/conf/config.xml -log_config=/data/app/engine/current/conf/log.xml
autostart=true
autorestart=true
startsecs=1
startretries=3
user=root
redirect_stderr=true
stdout_logfile=/data/app/engine/current/logs/console.out
environment=GODEBUG="gctrace=1"
```

## 5. git

cenos

```shell
1. 安装源
yum install http://opensource.wandisco.com/centos/6/git/x86_64/wandisco-git-release-6-1.noarch.rpm   // centos 6 git 1.x
- or -
yum install http://opensource.wandisco.com/centos/7/git/x86_64/wandisco-git-release-7-1.noarch.rpm   // centos 7 git 1.x
- or -
yum install http://opensource.wandisco.com/centos/7/git/x86_64/wandisco-git-release-7-2.noarch.rpm   // centos 7 git 2.x

2. yum install git 更新
3. git version 查看版本

***使用ius源安装
https://ius.io/setup

yum install https://repo.ius.io/ius-release-el7.rpm https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
测试
yum install yum-utils
搜索
yum info git2* |grep Name // 搜索git2.x版本
yum install git222  // 安装2.22
```

源码安装

```
** 更多信息参见git-note.md **

一般ubuntu使用apt， centos使用yum。特殊版本可以使用源码安装方式：(可以浏览目录查看最新版本)
wget https://mirrors.edge.kernel.org/pub/software/scm/git/git-2.9.5.tar.gz
tar -zxvf git-2.9.5.tar.gz
cd git-2.9.5
./configure
make

如果提示错误
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
```

源码安装

```
host: macos
docker: ubuntu 20.04.1 LTS
git: 2.28.0

apt install build-essential wget
apt install libssl-dev libcurl4-gnutls-dev zlib1g-dev autoconf tcl gettext
wget https://www.kernel.org/pub/software/scm/git/git-2.28.0.tar.gz
tar -zxf git-2.28.0.tar.gz
cd git-2.28.0
make configure
./configure --prefix=/usr/local/git
make -j8 
make install
```





## 6. redis

参考redis-note.md

```
安装
yum install redis
运行
redis-server /etc/redis.conf
或者
systemctl daemon-reload // 重新加载systemctl配置
systemctl restart redis

出错可以
"systemctl status redis.service" and "journalctl -xe" for details.

配置 /etc/redis.conf
bind 127.0.0.1 ::1 // 注释掉,否则只能在本机访问
requirepass <your-password>  // 需要密码
logfile /data/redis/log/redis.log // 日志文件。启动报Can't open the log file: Permission denied，需要修改目录权限到用户redis
dir /data/etc/redis // 工作目录. dump.rdb文件位置
bfilename dump.rdb // dump文件的名字.注意只能是文件名,不能含有路径

appendonly yes // aof启用
appendfilename "appendonly.aof" 
# Redis支持三种不同的刷写模式：
# appendfsync always #每次收到写命令就立即强制写入磁盘，是最有保证的完全的持久化，但速度也是最慢的，一般不推荐使用。
appendfsync everysec #每秒钟强制写入磁盘一次，在性能和持久化方面做了很好的折中，是受推荐的方式。
# appendfsync no     #完全依赖OS的写入，一般为30秒左右一次，性能最好但是持久化最没有保证，不被推荐。
 
#在日志重写时，不进行命令追加操作，而只是将其放在缓冲区里，避免与命令的追加造成DISK IO上的冲突。
#设置为yes表示rewrite期间对新写操作不fsync,暂时存在内存中,等rewrite完成后再写入，默认为no
no-appendfsync-on-rewrite no 
 
#当前AOF文件大小是上次日志重写得到AOF文件大小的二倍时，自动启动新的日志重写过程。
auto-aof-rewrite-percentage 100
 
#当前AOF文件启动新的日志重写过程的最小值，避免刚刚启动Reids时由于文件尺寸较小导致频繁的重写。
auto-aof-rewrite-min-size 64mb
 ———————————————— 
版权声明：本文为CSDN博主「aitangyong」的原创文章，遵循CC 4.0 by-sa版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/aitangyong/article/details/52072708
```

## 7. vsftpd

```
centos7.5 vsftpd
安装: yum install vsftpd -y
启动: service vsftpd start
配置文件
vim /etc/vsftpd/vsftpd.conf
# 建议
anonymous_enable=NO
# 可以考虑
ascii_upload_enable=YES
ascii_download_enable=YES

ftp默认使用系统账号密码登录
新建用户账户,用于ftp登录
# 新建用户ftp -d home -U 创建同名组 -p 加密后的密码,不是密码明文,建议用passwd确保密码正确
# -s指定登录shell为/sbin/nologin,表示禁止用户本地登录
adduser -d /data/ftp -U ftp -s /sbin/nologin
# 修改用户ftp的密码
passwd ftp  
# 修改用户ftp的登录shell
usermod -s /sbin/nologin ftp
如果有必要,可以修改 /data/ftp权限为777，以便其他本地用户访问
sudo chmod 777 /data/ftp
```

## 8. apache utils (ab测试)

```shell
安装: 
apt install apache2-utils
yum install httpd-tools
介绍:Apache HTTP Server (utility programs for web servers)
Provides some add-on programs useful for any web server. These include:

 - ab (Apache benchmark tool)
 - fcgistarter (Start a FastCGI program)
 - logresolve (Resolve IP addresses to hostnames in logfiles)
 - htpasswd (Manipulate basic authentication files)
 - htdigest (Manipulate digest authentication files)
 - htdbm (Manipulate basic authentication files in DBM format, using APR)
 - htcacheclean (Clean up the disk cache)
 - rotatelogs (Periodically stop writing to a logfile and open a new one)
 - split-logfile (Split a single log including multiple vhosts)
 - checkgid (Checks whether the caller can setgid to the specified group)
 - check_forensic (Extract mod_log_forensic output from Apache log files)
 - httxt2dbm (Generate dbm files for use with RewriteMap)
 
示例
ab -c 5 -n 1000 -p body.file -T 'application/json' http://172.16.55.183:2051/recommend
5并发,1000请求,Content-type: application/json, body使用文件body.file
```

## 9. python3

```
apt install python3
apt install python3-pip
## 安装 mysql-connector-python
## https://dev.mysql.com/doc/connector-python/en/connector-python-example-connecting.html
pip3 install mysql-connector-python

## centos
yum search -v <packagename>
# python34.x86_64 / python36.x86_64
yum list python3?.x86_64 
# python.x86_64 2.7
yum list python.x86_64
yum install python36
yum install python36-pip
```

## 10. wrk

```
Modern HTTP benchmarking tool
安装: (linux)
git clone https://github.com/wg/wrk.git
cd wrk
make
./wrk

wrk -t1 -c1 -d6s -T1s --script=recsimple.lua  http://172.16.200.74:2051/recommend

recsimple.lua 示例
wrk.method = "POST"
wrk.headers["Content-Type"] = "application/json"
wrk.body = '{"uid":"415825438", "count":8, "content_type":[1,3,12], "activate_timestamp":1454176960, "region":730000}'
```

## 11. GoReply

```
https://github.com/buger/goreplay/wiki/Getting-Started
https://cloud.tencent.com/developer/article/1354875
下载二进制或者本地编译

用法示例：
录制3000端口流量到文件requests_track.gor里。output-file-append表示文件不分块。文件名可以用 %Y%m%d%H%M%S.gor 形式表示。比如按照分钟，则每分钟生成一个文件。
需要root权限侦听端口，否则参考 https://github.com/buger/goreplay/wiki/Running-as-non-root-user
sudo ./gor --input-raw :3000 --output-file=requests_track.gor --output-file-append

回放requests_track.gor文件到服务http://localhost:9998。"requests_track.gor|200%"表示2倍流量复制。
gor --input-file "requests_track.gor" --output-http="http://localhost:9998"
```

## 12 gcc

```
linux下直接安装
apt/yum install gcc

windows下，需要安装MinGW-w64/CygWin
    https://www.zhihu.com/question/22137175/answer/90908473
    https://blog.csdn.net/yehuohan/article/details/52090282
	CygWin是windows下模拟linux环境的环境，有个模拟层。
	MinGW是windows下一致linux应用的开发环境，使用win32 api。
	MSYS是辅助MinGW的工具链。
	只是使用gcc，建议安装MinGW。参数：arch x86_64, thread posix, exception seh。
	
安装特定版本：
yum search gcc
yum install gcc-toolset-9-gcc-c++
scl list-collections // 查看有哪些toolset可用
scl enable gcc-toolset-9 bash
```

## 13 gdb pstack

```
yum install gdb
pstack工具是一个gdb脚本，使用gdb的bt或者thread apply all bt命令，通过sed工具输出线程堆栈信息
https://nanxiao.me/linux-pstack/
```

## 14 hadoop

```
https://hadoop.apache.org/docs/r1.0.4/cn/hdfs_shell.html
hadoop fs -cmd param
FS Shell:
cat chgrp chmod chown
copyFromLocal copyToLocal cp 
du dus expunge get getmerge ls lsr 
mkdir movefromLocal mv put 
rm rmr setrep stat 
tail test text touchz 
```

## 15 conda

```
anaconda: 约500M，集成1000+包
miniconda：conda内核，约80M
xxx2为集成python2， xxx3集成python3。支持win/linux/mac平台。

linux安装：下载相应的.sh（包含脚本和二进制文件）执行即可

no change     /usr/local/anaconda3/condabin/conda
no change     /usr/local/anaconda3/bin/conda
no change     /usr/local/anaconda3/bin/conda-env
no change     /usr/local/anaconda3/bin/activate
no change     /usr/local/anaconda3/bin/deactivate
no change     /usr/local/anaconda3/etc/profile.d/conda.sh
no change     /usr/local/anaconda3/etc/fish/conf.d/conda.fish
no change     /usr/local/anaconda3/shell/condabin/Conda.psm1
no change     /usr/local/anaconda3/shell/condabin/conda-hook.ps1
no change     /usr/local/anaconda3/lib/python3.7/site-packages/xontrib/conda.xsh
no change     /usr/local/anaconda3/etc/profile.d/conda.csh
modified      /home/cjx/.bashrc

==> For changes to take effect, close and re-open your current shell. <==

If you'd prefer that conda's base environment not be activated on startup, 
   set the auto_activate_base parameter to false: 

conda config --set auto_activate_base false



https://github.com/conda/conda
https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
https://repo.anaconda.com/archive/
https://repo.anaconda.com/archive/Anaconda3-2020.02-Linux-x86_64.sh
国内镜像(清华)
https://mirror.tuna.tsinghua.edu.cn/help/anaconda/

```

## 16 docker

```
macos安装：
brew cask install docker

添加国内源
daemon.json
  "registry-mirrors": [
    "http://hub-mirror.c.163.com"
  ],
  
docker run -it -h <docker_host_name> --cpus <N> -m 4G --network host -v <local_path>:<docker_path> image_name

查看镜像tag：（源是dockerhub时）
https://hub.docker.com/ 搜索对应镜像，查看tag
```

## 17 sftp同步

```
idea: setting， deployment， sftp自动同步
vscode: 添加sftp插件自动同步
windows： winscp，免费，配置puttykey，可登陆sftp。连接后，工具栏按钮Keep remote directory up-to-date(Ctrl-U)，start，可指定某个目录自动同步。窗口可缩小至系统托盘。
```

## 18 rsync

```shell
rsync --safe-links -arvzuc --exclude ".git" -e 'ssh -p 58422' . root@39.105.23.44:/home/cjx/tmp/dev/feature_process/ --chmod=D755,F644
// 同步本地local_dir，经过host1中转，到host2上面
rsync -ru -e 'ssh -p port1 user1@host1 ssh -p port2' local_dir user2@host2:<remote_dir>
如果使用--exclude-from=<list_file>；list_file里面每一行代表一个排除项

--chmod: 修改目标权限，D表示目录，F标识文件。
-v, --verbose 详细模式输出。
-q, --quiet 精简输出模式。
-c, --checksum 打开校验开关，强制对文件传输进行校验。
-a, --archive 归档模式，表示以递归方式传输文件，并保持所有文件属性，等于-rlptgoD。
-r, --recursive 对子目录以递归模式处理。
-R, --relative 使用相对路径信息。
-b, --backup 创建备份，也就是对于目的已经存在有同样的文件名时，将老的文件重新命名为~filename。可以使用--suffix选项来指定不同的备份文件前缀。
--backup-dir 将备份文件(如~filename)存放在在目录下。
-suffix=SUFFIX 定义备份文件前缀。
-u, --update 仅仅进行更新，也就是跳过所有已经存在于DST，并且文件时间晚于要备份的文件，不覆盖更新的文件。
-l, --links 保留软链结。
-L, --copy-links 想对待常规文件一样处理软链结。
--copy-unsafe-links 仅仅拷贝指向SRC路径目录树以外的链结。
--safe-links 忽略指向SRC路径目录树以外的链结。
-H, --hard-links 保留硬链结。
-p, --perms 保持文件权限。
-o, --owner 保持文件属主信息。
-g, --group 保持文件属组信息。
-D, --devices 保持设备文件信息。
-t, --times 保持文件时间信息。
-S, --sparse 对稀疏文件进行特殊处理以节省DST的空间。
-n, --dry-run现实哪些文件将被传输。
-w, --whole-file 拷贝文件，不进行增量检测。
-x, --one-file-system 不要跨越文件系统边界。
-B, --block-size=SIZE 检验算法使用的块尺寸，默认是700字节。
-e, --rsh=command 指定使用rsh、ssh方式进行数据同步。
--rsync-path=PATH 指定远程服务器上的rsync命令所在路径信息。
-C, --cvs-exclude 使用和CVS一样的方法自动忽略文件，用来排除那些不希望传输的文件。
--existing 仅仅更新那些已经存在于DST的文件，而不备份那些新创建的文件。
--delete 删除那些DST中SRC没有的文件。
--delete-excluded 同样删除接收端那些被该选项指定排除的文件。
--delete-after 传输结束以后再删除。
--ignore-errors 及时出现IO错误也进行删除。
--max-delete=NUM 最多删除NUM个文件。
--partial 保留那些因故没有完全传输的文件，以是加快随后的再次传输。
--force 强制删除目录，即使不为空。
--numeric-ids 不将数字的用户和组id匹配为用户名和组名。
--timeout=time ip超时时间，单位为秒。
-I, --ignore-times 不跳过那些有同样的时间和长度的文件。
--size-only 当决定是否要备份文件时，仅仅察看文件大小而不考虑文件时间。
--modify-window=NUM 决定文件是否时间相同时使用的时间戳窗口，默认为0。
-T --temp-dir=DIR 在DIR中创建临时文件。
--compare-dest=DIR 同样比较DIR中的文件来决定是否需要备份。
-P 等同于 --partial。
--progress 显示备份过程。
-z, --compress 对备份的文件在传输时进行压缩处理。
--exclude=PATTERN 指定排除不需要传输的文件模式。
--include=PATTERN 指定不排除而需要传输的文件模式。
--exclude-from=FILE 排除FILE中指定模式的文件。
--include-from=FILE 不排除FILE指定模式匹配的文件。
--version 打印版本信息。
--address 绑定到特定的地址。
--config=FILE 指定其他的配置文件，不使用默认的rsyncd.conf文件。
--port=PORT 指定其他的rsync服务端口。
--blocking-io 对远程shell使用阻塞IO。
-stats 给出某些文件的传输状态。
--progress 在传输时现实传输过程。
--log-format=formAT 指定日志文件格式。
--password-file=FILE 从FILE中得到密码。
--bwlimit=KBPS 限制I/O带宽，KBytes per second。
-h, --help 显示帮助信息。
```

## 19 inotify

```
用途：监控文件变化
apt install inotify-tools

监控：
inotifywait --exclude '\.(part|swp)' -r -mq -e  modify,create,delete <file/directory-to-watch>
监控一次：
inotifywait --exclude '\.(part|swp)' <file/directory-to-watch>

配合rsync，自动同步到远端
while true
do
	inotifywait <dir>
	rsync xxx
done
```

## 20 sysvinit-tools

```
yum install sysvinit-tools
Tools used for process and utmp management.
包括
killproc pidof等
```

## 21 tmux

```shell
终端复用器：分离ssh和terminal。每个session可以由数字标识，或者自定义名字
tmux new -s <session-name>
tmux detach
tmux attach -t <session-name>
tmux rename -t n <session-name>
tmux ls // 列出会话
tmux kill-session -t n

快捷键
Ctrl+b d：分离当前会话。
Ctrl+b s：列出所有会话。
Ctrl+b $：重命名当前会话。
```

## 22 iproute

```
yum install iproute
```

## 23 docker

```shell
docker run -d -e 'CONSUL_LOCAL_CONFIG={"skip_leave_on_interrupt": true}' --name=node3 consul agent -server -bind=172.17.0.4  -join=172.17.0.2 -node-id=$(uuidgen | awk '{print tolower($0)}')  -node=node3 -client=172.17.0.4


docker run -it -d -e 'CONSUL_LOCAL_CONFIG={"skip_leave_on_interrupt": true}' --name=node5 consul agent -server -bind=172.17.0.6  -join=172.17.0.2 -node-id=$(uuidgen | awk '{print tolower($0)}')  -node=node5


docker run -it -h <cntr_host_name> -v <host_dir>:<cntr_dir> --network host centos:7
docker exec -it <img_id> zsh

docker run --net=host -h lambda --name yien -it -d --entrypoint /bin/bash docker-reg.devops.xiaohongshu.com/lambda/lambda-service:dev -c 'sleep infinity' 
```

## 24 bazel

```shell
bazel build //<path-to-pkg>:<pkg-name>
// 打印依赖图，可以在http://www.webgraphviz.com中展示
bazel query --notool_deps --noimplicit_deps "deps(//main:hello-world)" --output graph
// 使用xdot工具显示依赖图。注意会在弹出窗口显示，知道关闭窗口才返回。需要安装graphviz和xdot
xdot <(bazel query --notool_deps --noimplicit_deps "deps(//main:hello-world)" --output graph) 
```

## 25 gpg

```shell
brew install gpg
gpg签名验证
一般提供gpg签名文件的同时，会附带说明public key id及公钥文件。

gpg --import <gpg-key-file> // 从文件导公钥
gpg --recv-keys <key-id>    // 从服务器导入公钥
gpg --verify xxx.gpg xxx 验证签名
```



# SHELL脚本

## 1. 规范

### 1.1语法

#### 基础

```
#!/bin/bash
“#!”指定解释器。不要加空格。

set -o errexit // 遇到错误自动退出
set +e // 暂时关闭错误自动退出

set -e # set -o errexit 简写形式
set -u # 变量未设置退出
set -x # debug
set -v # verbose

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
#### 条件判断

```shell
**条件判断condition
[ 其实是一个程序（ /usr/bin/[ ，有时是/usr/bin/test的软链接)。因此使用时后面要加空格，以]表示输入结束，结束时设置返回值为表达式的值。详情可以参考 man [。例如(注意空格！)：
[ $foo = "bar" ]
格式：
[ EXPRESSION ]	# 常用
[ ]				# 默认为false
[ OPTION		# 如help或version

参数
-a and条件 如 [ -w a.txt -a -w b.txt ] 两个文件都可写
-o or条件
! not条件

一元操作符
() , ! // 表达式
-n , -z // 字符串（非空/空字串）。注意数值不是空字符串。

二元操作符
= , !=  // 字符串
-a (and), -o (or) // 表达式
-eq , -ne , -ge , -gt , -le , -lt  // 数值
（经测试，= !=可以用在数值上，-eq -ne也可以用在字符串上。似乎shell会在数值和字串之间简单转换？）
如果想判断一个参数是不是数值,可以用-ge , -gt , -le , -lt,非数值if会出错,跳转到else分支

另外，还有判断文件的一些操作
-a file exists.
-e file exists (just the same as -a).
-d file exists and is a directory.
-f file exists and is a regular file.
-s file exists and has a size greater than zero.
-r file exists and is readable by the current process.
-w file exists and is writable by the current process.
-x file exists and is executable by the current process.

例如：
if [ -f xxx ]; then
  echo file xxx exists
else
  echo file xxx not exist
fi

if [ ! -f xxx ]; then
  echo file xxx not exist
fi
```
#### 带参数脚本

```
# 第一个参数赋值给arg1
arg1=$1 
# arg1文件名不存在
if [ ! -e $arg1 ];then
    echo file \"$fn\" not exist
    exit
fi
```





#### 流程控制

```
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

for ((i:=0;i<10;i++))

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
#### 请求用户输入

```
echo -n "please input: "
read input
echo "input is ${input}"
```

#### 读取文件内容

```
读取整个文件（如只有一行）
data=$(cat file.txt)
echo ${data}

逐行读取
cat file.txt | while read line
do
    echo $line
done

按照IFS指定的分隔符读取。IFS的默认值为：空白(包括：空格，制表符，换行符)．
for var in `cat file.txt`
do
	echo $var
done

使用awk处理。可通过system()调用shell命令
```

#### 数组

```
var=(1 2 3 4) // ()， 空格分隔
echo len=${#var[@]}
for i in $var[@]; do
	echo $i
done
```



#### 关联数组(字典)

```
#声明associative arrays（类似字典）
declare -A y
y["sss"]="kkk"
y["aaa"]="bbb"
#获取所有键key
echo ${!y[@]} 
#获取所有键val
echo ${y[*]}
#获取指定键的值
echo ${y["sss"]}
```

#### 多进程并行

```
for xxx;
do 
{
	// code run in new proc
	while true;
	do
		xxx // run for ever. eg. monitor & sync
	done
} & 
done 
wait // wait til all done
```



#### 其它

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

##数值计算
r=`expr 4 \* 5` 
r=$(( 4 * 5 ))
r=$[ 4 * 5 ]
// 幂运算2^3 (expr没有幂运算)
r=$(( 2 ** 3 ))
r=$[ 2 ** 3 ]

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
SHELL_CMD='ssh -p 58422'
HOST=root@192.168.106.186
echo $SHELL_CMD $HOST '"' "mkdir -p" $remote '"' 
// ssh -p 58422 root@39.105.23.44 " mkdir -p /home/cjx/tmp/dev/d1/d2/d3 "

**调试
bash -x shell param

**自增
例子
```

## 1.4 示例

### 管道遍历文件并处理

```
ls *.proto | cat | awk '{print "protoc --cpp_out=. " $1 | "/bin/bash"}'
列出所有proto文件 | 分行 | awk处理
awk：打印命令 | 传递给bash
awk也可以使用system调用命令
```





# gdb

## core dump

```
ulimit -c unlimited // 不限制core大小
gdb -c core.xxxx
bt
file xxx 加载文件符号
frame n 或者 f n 切换到bt的第n个帧
i f // info frame
i t // info thread 列出线程信息
t n // 切换到第N个线程
directory <src directory> // 添加源码搜索路径。No such file or directory 时
p var-name // print变量值
```

# 常用命令

```
遍历文件并处理
ls *.proto | cat | awk '{print "protoc --cpp_out=. " $1 | "/bin/bash"}'
find ./ -name \*.proto | awk '{print "protoc --cpp_out=. " $1 | "/bin/bash"}'


```

