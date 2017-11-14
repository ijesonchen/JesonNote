---
title: win10 How-to
date: 2017-10-05 23:14:00
tags: [win10, how to]
---
** win10 hot to**

<!--more-->

# linux subsystem with ssh service

how to install linux subsytem and start ssh service

## 安装

- 启用开发者模式

- 添加功能linux子系统

- 命令行 lxrun /install或者bash


安装是会提示错误0x80072ee2，是由于网络不稳定。换个时间再试，或者启用VPN，或者开启shadowsocks的全局代理。

## 启用root

`sudo passwd root`

设置密码后既可启用root

进入root账户

`su`

## ssh服务

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

# 更改网络类型(公用、专用、私有)

[4 Ways To Change Network Type In Windows 10 (Public, Private Or Domain)](https://www.itechtics.com/change-network-type-windows-10/)

## 使用本地安全策略(secpol.msc)

网络列表管理器策略，双击相应网络修改

# Hyper-v开启远程管理

[WINDOWS10 2012 2016 HYPER-V 实现管理器远程管理虚拟机](http://www.junww.com/server/2017/0422/237.html)

**NOTE**

​	服务器端正常配置，客户端连接时提示权限问题。

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

