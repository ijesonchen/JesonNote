---
title: linux how to
date: 2017-05-24 22:14:00
tags: [linux, how-to]
---

**linux how to**

linux how to

<!--more-->

[TOC]

# 安装

```
虚拟机/服务器安装，建议选择英文，否则命令行界面容易显示乱码
```





# ubuntu常用命令

```
sudo su		// 进入root账户
apt intall	// 安装软件
```



# windows无法使用hostname ping通linux服务器

**NOTE**

​	server安装选中smaba服务，可以再windows下使用hostname ping通

​	desktop使用apt安装samba后，samba start启动失败，smbd启动成功，但是无法使用hostname ping

原因：linux默认未开启wins服务

解决：安装samba自动开启相应服务

查看linux host name: `more /etc/hosts`

ref: 

[局域网中window可以解析hostname，而linux却不可以？](http://blog.csdn.net/lewif/article/details/74546045)

[[深入理解Linux修改hostname](http://www.cnblogs.com/kerrycode/p/3595724.html)](http://www.cnblogs.com/kerrycode/p/3595724.html)

[Ubuntu16.04安装Samba](http://www.cnblogs.com/bencakes/p/5541771.html)



