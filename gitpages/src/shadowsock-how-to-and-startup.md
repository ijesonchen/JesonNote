---
title: how to config shadow socks server and config auto startup
date: 2017-09-30 15:55:00
tags: [shadow socks, auto strtup, ubuntu]
---



## ubuntu 16.04下安装shadow socks服务器并配置自动启动
更新pip后安装ssserver，使用systemctl启用rc.local，在rc.local添加配启动脚本
<!--more-->

- ### 安装shadow socks服务

  ```
  apt install python-pip
  pip install shadowsocks
  ```

  ​

  更新pip到最新版本，并安装ssserver


- ### 相关命令

ssserver -p $port$ -k $password$ -m aes-256-cfb -d start

ssserver -d stop

- ### 自动启动


启用/etc/rc.local: `systemctl enable rc.local`

编辑rc.local `vim /etc/rc.local`

在`exit 0`前面添加`/usr/bin/python /usr/local/bin/ssserver -p $port$ -k $password$ -m aes-256-cfb -d start`

