---
title: win10 linux subsystem
date: 2017-10-05 23:14:00
tags: [win10 linux]
---
# win10 linux subsystem with ssh service
how to install linux subsytem and start ssh service
<!--more-->

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





