---
title: win10 How-to
date: 2017-10-05 23:14:00
tags: [win10, how to]
---
** win10 hot to**

<!--more-->



# WSL

linux subsystem with ssh service

how to install linux subsytem and start ssh service

## 1. 安装

- 启用开发者模式

- 添加功能linux子系统

- 命令行 lxrun /install或者bash


安装是会提示错误0x80072ee2，是由于网络不稳定。换个时间再试，或者启用VPN，或者开启shadowsocks的全局代理。

## 2. 启用root

`sudo passwd root`

设置密码后既可启用root

进入root账户

`su`

## 3. ssh服务

```
apt install openssh-server
sudo service ssh start | stop | restart
```

配置项 

```
* /etc/ssh/ssh_config
PasswordAuthentication yes  # 启用密码登录

* /etc/ssh/sshd_config
PermitRootLogin yes # 允许root通过ssh登录, prohibit-password 禁用
PasswordAuthentication yes # 启用密码登录

* /root/.ssh/authorized_keys
追加sshkey到该文件可实现自动登陆
cat id_rsa.pub >> /root/.ssh/authorized_keys  # 登录到root的key
```

## 4. 常见问题

windows下运行wsl命令

```
直接在windows命令行下通过wsl运行，如 wsl ls -l
如果wsl安装了go，可以通过
wsl /usr/local/go/bin/godoc -http=:6060 
启动godoc server
```

wsl2 VMMEM消耗内存过高

```
https://github.com/microsoft/WSL/issues/4166
原因: 可能是因为wsl2虚拟机问题,导致只申请内存不释放.
解决1. windows下wsl --shutdown关闭虚拟机,然后wsl重新进入虚拟机
解决2. 配置wslconfig限制最大内存使用
更多说明:
https://docs.microsoft.com/en-us/windows/wsl/release-notes#build-18945

新建文本文件 %UserProfile%\.wslconfig
(例如用户admin就新建文本文件 c:\users\admin\.wslconfig)
内容:
[wsl2]
memory=2GB
swap=0
localhostForwarding=true
```

wsl和wsl2磁盘性能区别

```
wsl文件存放在windows磁盘上，所有磁盘操作需要通过windows系统，性能较差。
wsl2使用一个轻量虚拟机实现，实际运行在虚拟机里面，有自己的虚拟磁盘（文件ext4.vhdx），通过9p协议和windows通信(访问路径\\wsl$)，可以实现完整linux功能。因此在wsl2里面访问自身系统文件（如/home,/bin等）效率较高，但是访问/mnt/c等挂载的windows磁盘，性能可能比wsl更低(可能是优化问题)。但是相应的，如果在windows下通过网络路径\\wsl$访问wsl2磁盘文件,性能也很低。
目前没有好到两个系统同时访问但是性能没有降低的方案。例如在windows下用intellij写程序，但是git，编译等在wsl下。不论实际文件存储在windows还是wsl,另一个系统访问都要通过宿主系统,这时候磁盘性能会急剧降低,尤其是涉及大量小文件操作时。
```

# 多显示器任务栏现实相同系统托盘

```
win10 v1903为提供相关支持。可以考虑第三方工具DisplayFusion，有免费版可用
http://www.displayfusion.com/Features/Taskbar/ 
参考链接
https://answers.microsoft.com/zh-hans/windows/forum/all/win10%E5%8F%8C%E6%98%BE%E7%A4%BA%E5%99%A8%E5%A6%82/619e04bd-78aa-4930-a528-6f1c804f60b3
```

# 更改网络类型(公用、专用、私有)

[4 Ways To Change Network Type In Windows 10 (Public, Private Or Domain)](https://www.itechtics.com/change-network-type-windows-10/)

使用本地安全策略(secpol.msc)

网络列表管理器策略，双击相应网络修改

# Hyper-v开启远程管理

## 兼容性问题

```
目前（2019.11），hpyer-v和其他虚拟机（vmware，virtual box）不兼容，会导致后者无法启动。原因是hyper-v启动后，整个windows子系统是运行在虚拟机平台上，会导致其他虚拟机启用虚拟特征时失败。据称virtualbox已经支持hyper-v，vmware也在2019.8发表blog正在进行支持工作。但是目前仍然没找到可以用的兼容版本。
```

[WINDOWS10 2012 2016 HYPER-V 实现管理器远程管理虚拟机](http://www.junww.com/server/2017/0422/237.html)

```
**NOTE**
服务器端正常配置，客户端连接时提示权限问题。

一、启用WinRM服务（服务器、客户端同时启用）
Windows Remote Management (WS-Management)
二、服务器以管理员方式启动CMD，运行：winrm quickconfig
三、客户端以管理员方式启动CMD，输入powershell回车，然后运行：
net start winrm
Set-Item wsman:\localhost\Client\TrustedHosts -Value 192.168.1.2     (换成你的服务器IP)
将会提示：
此命令修改 WinRM 客户端的 TrustedHosts 列表。TrustedHosts列表中的计算机可能不会经过身份验证。该客户端可能会向这些计算机发送凭据信息。是否确实要修改此列表?
[Y] 是(Y) [N] 否(N) [S] 暂停(S) [?] 帮助 (默认值为“Y”): Y
四、进入Hyper-V管理器添加远程服务器，敲入服务器的IP即可正常管理。注意账户名是 ip\user 的形式，否则会提示用户名无效。
```
## 命令行问题

命令行下exe命令无法执行，提示找不到命令。

```
** 现象 **
1. openjdk解压后 java命令无法执行。添加path后也不行。进入到openjdk\bin目录下执行java也不能执行，但java.exe可以执行。
2. python27目录下命令可以执行

** 处理 **
1. 搜到有说PATHEXT问题。默认环境变量PATHEXE=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC;.PY;.PYW。检查正常。
2. 发现无法识别的命令是小写，但是PATHEXT是大写，怀疑大小写问题。修改环境变量PATHEXE=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC;.PY;.PYW;.exe 后解决。但是具体原因未查到，不能解释其他小写能够执行的原因。
```





