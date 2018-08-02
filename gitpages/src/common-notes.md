---
title: notes
date: 2018-08-03 00:00:00
tags: [note]
---

**Notes**

一些记录

<!--more-->

[TOC]

# 安装

软件安装记录

## Rabbitmq

环境macos high sierra 10.13.5

```
安装：brew install rabbitmq
启动：brew services start rabbitmq
界面：http://localhost:15672 账号/密码默认都是guest
路径：/usr/local/opt/rabbitmq/sbin (插件管理等)
插件安装：http://www.rabbitmq.com/installing-plugins.html
	http://www.rabbitmq.com/community-plugins.html
	第三方：下载插件ez文件放到 /usr/local/Cellar/rabbitmq/version/plugins中
	rabbitmq_delayed_message_exchange-20171201-3.7.x.ez
	执行: ./rabbitmq-plugins  enable rabbitmq_delayed_message_exchange
```

## MongoDB

环境ubuntu 14.04，命令行

```
wget [mongodb_bin_url]
tar 解压
如果碰上libc版本不对，需要变异新的libc
```













