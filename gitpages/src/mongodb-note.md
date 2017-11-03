---
title: mongodb note
date: 2017-05-24 22:14:00
tags: [tag1, tag2]
---

**mongodb note**

<!--more-->

[TOC]

# Start

mongod --dbpath [dbpath] --logpaath  [logfilename]

for windows:

```
mongod --dbpath "D:\MongoDB\data" --logpath "D:\MongoDB\log\mongolog.log" --install --serviceName "MongoDB"
```

# OS: windows or linux

Mongodb perfors better in linux than windows, especially when insert.

Bulk insert: windows server 08R2 ~10K rps, centos 7 ~35K rps.

# Client

- Robomongo: simple to use, suggested if familiar with mongodb console
- mongo chief: advanced usage, query wizard

# Driver

- C driver is efficient, no independent, but vorbose than C++ (resource management)
- C++ should compile with boost
- others: not familiar

suggest: RAII warpper with C driver.

# Console

`mongo ip:port`

ref: [[MongoDB使用小结：一些常用操作分享](http://www.cnblogs.com/cswuyg/p/4595799.html)](http://www.cnblogs.com/cswuyg/p/4595799.html)

## Show slow operation

```
db.currentOp().inprog.forEach( 
  function(item) { 
	if(item.secs_running<1000) print(JSON.stringify(item)) }
  )
```

or just `print( item.client )`







