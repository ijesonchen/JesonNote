---
title: SQL NOTE
date: 2019-02-24 00:00:00
tags: [sql, note]
---

**SQL入门笔记**

w3cschool学习SQL的一些笔记

<!--more-->

[TOC]

# SQL

## **SELECT** - extracts data from a database

```
SELECT (DISTINCT) col,...|* FROM table_name
SELECT COUNT (DISTINCT colunm_name) FROM table_name
SELECT COUNT(*) AS result_name FROM (SELECT DISTINCT column_name from maintainable
SELECT * FROM table_name WHERE condition
	=  <>  !=  >  <  >=  <=  BETWEEN  LIKE  IN
	AND  OR NOT  ()
SELECT * FROM table_name ORDER BY col,... ASC|DESC

INSERT INTO table_name (col,...) VALUES (val,...)
	如果所有列都有值，可以忽略列名
	
NULL: IS NULL, IS NOT NULL

UPDATE table_name SET col=val,... WHERE condition
	省略WHERE则更新全部记录
	
DELETE FROM table_name WHERE condition
	省略WHERE则删除所有记录
	
TOP/LIMIT/ROWNUM
	MS: SELECT TOP number|percent col,... FROM table_name WHERE condition
	MySQL: SELECT col,... FROM table_name WHERE condition LIMIT number
	Oracle: SELECT col,... FROM table_name WHERE condition AND ROWNUM <= number

functions：process on values of A column. usually with AS
	MIN/MAX/COUNT/AVG/SUM
	SELECT FUNC(col) AS new_col FROM table_name
	
wildcard: in LIKE pattern
	% (0,1 or more) 
	_ (exactly 1)
	[chars] (match chars) MS only
	[!chars] / [^chars] (not match chars) MS only
	

```



## **UPDATE** - updates data in a database
## **DELETE** - deletes data from a database
## **INSERT INTO** - inserts new data into a database
## **CREATE DATABASE** - creates a new database
## **ALTER DATABASE** - modifies a database
## **CREATE TABLE** - creates a new table
## **ALTER TABLE** - modifies a table
## **DROP TABLE** - deletes a table
## **CREATE INDEX** - creates an index (search key)
## **DROP INDEX** - deletes an index

# mysql函数

```
https://www.w3resource.com/mysql/date-and-time-functions/date-and-time-functions.php
```



# Q&A

## show processlist 排查问题

```
https://xu3352.github.io/mysql/2017/07/08/msyql-show-full-processlist // processlist表使用方法
https://blog.csdn.net/21aspnet/article/details/6922190 // processlist.state状态详解
https://www.tutorialspoint.com/how-do-i-kill-all-the-processes-in-mysql-show-processlist

-- 查看当前线程处理情况
show full processlist 

-- 添加过滤条件
select id, db, user, host, command, time, state, info
from information_schema.processlist
where command != 'Sleep'
order by time desc 

-- 杀掉一个线程
kill <id>;

-- 查询执行时间超过2分钟的线程，然后拼接成 kill 语句
select concat('kill ', id, ';')
from information_schema.processlist
where command != 'Sleep'
and time > 2*60
order by time desc 

-- 命令行
mysql -e "show full processlist;" -ss | awk '{print "KILL "$1";"}'| mysql
```

## 连接错误: not allowed

```
ERROR 1130 (HY000): Host '116.228.147.110' is not allowed to connect to this MySQL server

https://confluence.atlassian.com/jirakb/configuring-database-connection-results-in-error-host-xxxxxxx-is-not-allowed-to-connect-to-this-mysql-server-358908249.html
https://stackoverflow.com/questions/1559955/host-xxx-xx-xxx-xxx-is-not-allowed-to-connect-to-this-mysql
原因:安全配置,默认root只能本机登录
解决1> 新建其他用户
  CREATE USER 'user'@'%' IDENTIFIED BY 'password';
  GRANT ALL PRIVILEGES ON *.* TO 'user'@'%' WITH GRANT OPTION;
  FLUSH PRIVILEGES;
解决2> 添加例外ip
  USE mysql;
  SELECT user,host FROM user;
  GRANT ALL PRIVILEGES ON *.* TO root@<ipaddr> IDENTIFIED BY '<rootpwd>' WITH GRANT OPTION;
```



# MySQL安装

mysql 8.0在win10下的安装，采用zip包方式。installer方式更加简单一些。

```
0. 环境
windows 10版本1803
参考 http://www.runoob.com/mysql/mysql-install.html 略有改动，注意配置文件部分。
1. 下载community版本,zip包
地址：https://dev.mysql.com/downloads/mysql/
下载Windows (x86, 64-bit), ZIP Archive。可能需要注册一个Oracle账号（免费）。
将文件解压到d:\bin\mysql，注意目录结构（d:\bin\mysql下面不要再有一级文件夹）。可以在目录下新建一个文本文档写明详细版本号（zip文件的名字）
2. 新建配置文件
d:\bin\mysql\conf.ini。参见示例，注意**部分
默认情况下会按照一定顺序搜索搜索my.ini，具体可参考mysqld帮助。
3. 初始化并启动服务
以管理员身份打开cmd，进入目录d:\bin\mysql\bin
查看命令帮助 mysqld --verbose --help
注意如果指定配置文件，defaults-file参数必须是第一个参数（参见帮助）

mysqld --defaults-file=d:\bin\mysql\conf.ini --initialize --console // 初始化.注意会生成root密码
mysqld install // 安装服务
net start mysql // 启动服务
mysql -h127.0.0.1 -uroot -p<pwd> // 连接mysql
```

conf.ini示例

```
[mysql]
# 设置mysql客户端默认字符集 **
default-character-set=utf8mb4
 
[mysqld]
# 设置3306端口
port = 3306
# 设置mysql的安装目录
basedir=d:\\bin\\mysql
# 设置 mysql数据库的数据的存放目录，MySQL 8+ 不需要以下配置，系统自己生成即可，否则有可能报错
# datadir=d:\\bin\\mysql\\data
# 允许最大连接数
max_connections=20
# 服务端使用的字符集默认为8比特编码的latin1字符集
character-set-server=utf8mb4
# 创建新表时将使用的默认存储引擎
default-storage-engine=INNODB
# error-message file **
lc_messages_dir=d:\\bin\\mysql\\share\\english
# 默认加密方式mysql_native_password。caching_sha2_password部分客户端不支持。 **
default-authentication-plugin=mysql_native_password
```

