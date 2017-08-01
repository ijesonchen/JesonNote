---
title: python note
date: 2017-05-24 22:14:00
tags: [python, note]
---

python笔记
<!--more-->


[TOC]


# 语法

## gobal关键词引用全局变量
```language
idx = 0
def P():
	print(idx)
```
会报错
```
local variable 'idx' referenced before assignment
```
修正
```language
idx = 0
def P():
	global idx
	print(idx)
```

## 任意键继续
```language
input('press any key to continue')
```

# 库
## math和cmath
math包含取整、三角函数、幂、指数、对数、阶乘、伽马、最大公约数等，并包含一些常量定义（e,pi)
cmath和math类似，但是支持复数，包括三角、幂指一级坐标变换等（极坐标）

# 文件操作

## os.scandir 快速遍历目录
imort os
遍历目录
os.listdir() 较慢
for ent in os.scandir() 较快，返回DirEntry
	ent.is_dir()
	ent.is_file()
	ent.name
	ent.path
	ent.stat() 可以获取文件时间
		
## zipfile 压缩打包
import zipfile
from zipfile imort *
with ZipFile(zipname, 'w') as zipFile:
	for x in os.scandir(dirname):
		fullname = base + '\\' + x.name
		zipFile.write(fullname, arcname = x.name, compress_type = zipfile.ZIP_DEFAULTED)
	
# 进程及线程

## multiprocessing.Pool 进程池

from multiprocessing import Pool

def f(x):
    return x*x

if __name__ == '__main__':
	x = [1, 2, 3]
    with Pool(5) as p:
        print(p.map(f, x))

# 数据库

## pymongo 编译及导入
python setup.py build，生成编译后py文件，复制到脚本目录中即可直接import

## mongo 查找
```language
client = pymongo.MongoClient(ip, port)
db = client.dbname
table = db[tablename]
for x in table.find().sort(fieldname, pymongo.ASCENDING)
	id = x.['id']
	...
rec = table.find_one({field : value})
```

## pymongo 插入
```language
def Insert():
    client = MongoClient(HOST)
    db = client[DB]
    coll = db[COLL]
    for i in range(100):
        rec = GenRecord()
        input("any key to continue")
        coll.insert(rec)
```

## pymongo 批量插入
可以使用bulkoperation或者直接insert[]
```language
def Insert():
    client = MongoClient(HOST)
    db = client[DB]
    coll = db[COLL]
    recs = []
    for i in range(100):
        rec = GenRecord()
        input("any key to continue")
        recs.append(rec)
    coll.insert(recs)
```