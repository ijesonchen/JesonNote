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

## 二进制raw数据处理

- 二进制字面值 `x = b'\x01\x02'`

- utf-8解码
```
import codes
codecs.decode(bindata, 'utf-8')
```

- 解码读写文件 `codecs.open(filename, mode='r', encoding=None, errors='strict', buffering=1) `

## 编码

```
https://blog.csdn.net/ran337287/article/details/56298949
ansi
unicode u''
utf8 '\x??'
转换函数 ord()与 chr()、unichr()
对象方法： encode(), decode()
```

## for

```
arr = []
for data in arr:
  print(data)
  
for i in range(len(arr)): # range all
  print(arr[i])
  
for i in range(10, 20): # range [10, 20)
  print(arr[i])
```



# 库

## math和cmath
math包含取整、三角函数、幂、指数、对数、阶乘、伽马、最大公约数等，并包含一些常量定义（e,pi)
cmath和math类似，但是支持复数，包括三角、幂指一级坐标变换等（极坐标）

# 安装

## python

```
python.org下载安装程序。建议python3。老项目可能要python2。可以同时安装，建议先python2， 再python3。常用的版本加入PATH路径。
安装完后，建议升级pip，否则经常会出现pip install失败：
python -m pip install --upgrade pip

安装pylint。会安装到在%python_path%\script\pylint.exe
pip install pylint

安装pipenv。（有人反馈不完善不好用2019.9）。pipenv 是 python 官方推荐的包管理工具，集成了 virtualenv、pyenv 和 pip 三者的功能于一身，类似于 php 中的 composer。
pip install --user pipenv

** utuntu
apt install python-pip
apt install python3-pip
```

## 更换国内源

```
PIP 更换国内安装源
https://blog.csdn.net/yuzaipiaofei/article/details/80891108
2018-07-02 22:58:20 SoloLinux 阅读数 95261  收藏 更多 
pip国内的一些镜像

  阿里云 http://mirrors.aliyun.com/pypi/simple/ 
  中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/ 
  豆瓣(douban) http://pypi.douban.com/simple/ 
  清华大学 https://pypi.tuna.tsinghua.edu.cn/simple/ 
  中国科学技术大学 http://pypi.mirrors.ustc.edu.cn/simple/

修改源方法：

临时使用： 
可以在使用pip的时候在后面加上-i参数，指定pip源 
eg: pip install scrapy -i https://pypi.tuna.tsinghua.edu.cn/simple

永久修改： 
linux:  修改 ~/.pip/pip.conf
window: %HOMEPATH%\pip\pip.ini

[global]
timeout = 6000
index-url = http://mirrors.aliyun.com/pypi/simple/
trusted-host = mirrors.aliyun.com
```



## Intellij IDEA设置

```
0. 安装python运行环境。
1. file, settings, plugins，搜索python，安装。
2. file, Project Structure, Platform Settings, SDKs， 添加python sdk， 设置Systm Interpreter。
3. file, Project Structure, Project Settings, Modules, 选中项目，python，设置Python Interpreter。
```



## 库的安装

```
升级pip
linux: pip install --upgrade pip
windows: python -m pip install --upgrade pip
安装包
pip install package
```

## 库安装时缺少文件

python.h

```
pip install mysql-python
提示缺少python.h
解决:
	centos: yum install python-devel
	ubuntu: apt install python-dev
```

## 常用库

```
python3:
redis
PyMySQL
pymongo
```

### redis

```
import redis

# redis config
rhost = "xxx"
rport = 6379
rdb = 0
rpwd = ""
keys = ["hot_item_total_0", "hot_item_total_1", "hot_item_total_2"]
 
def redis_test():
    r = redis.Redis(host=rhost, port=rport, db=rdb, password=rpwd)
    for key in keys:
        # get []
        r.lrange(key, 0, 500)
```

### PyMySQL

```
import pymysql

# mysql config
shost = ":3306" 
sdb = ""
suser = ""
spwd = ""

def mysql_test():
    db = pymysql.connect(shost, suser, spwd, sdb)
    cursor = db.cursor()
    cursor.execute("select goods_id, is_onsale, is_deleted, pay_rmb from goods where goods_id=10012")
    data = cursor.fetchall()
    print(data)
    db.close()
    pass
```

### pymongo

```

```



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
```
import zipfile
from zipfile imort *
with ZipFile(zipname, 'w') as zipFile:
	for x in os.scandir(dirname):
		fullname = base + '\\' + x.name
		zipFile.write(fullname, arcname = x.name, compress_type = zipfile.ZIP_DEFAULTED)
```

## 读写json

```
import json

with open("hot_item_state.pwd", "r") as f:
    tmp = json.loads(f.read())
    print(tmp)
    print(tmp["rpwd"])
    
    jsonstr = json.dumps(test_dict)
    f.write(jsonstr)
    json.dump(new_dict,f)
```



# 进程及线程

## multiprocessing.Pool 进程池

```
from multiprocessing import Pool

def f(x):
    return x*x

if __name__ == '__main__':
	x = [1, 2, 3]
	with Pool(5) as p:
	    print(p.map(f, x))
```

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
# 网络

## 下载文件，获取下载速度，进度等

使用pycurl `pip install pycurl`

开始回调progress时，download_t (total byte to download)可能为0。

```
tm_init = time.time()
tm_start = time.time()
tm_last = time.time()
total_byte = 0

def progress(download_t, download_d, upload_t, upload_d):
	...
def down(url):
    tm_init = time.time()
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.NOPROGRESS, False)
    c.setopt(c.XFERINFOFUNCTION, progress)
    c.setopt(pycurl.WRITEFUNCTION, body)
    c.setopt(pycurl.VERBOSE,0)
    c.perform()
    tm_down = time.time()
    cost1 = tm_down - tm_init
    cost2 = tm_down - tm_start
    print("cost1 %.2f, cost2 %.2f, kbps %d %d"%(cost1, cost2, total_byte / 1024 / cost2, sum(x)/len(x)))
```



## ping服务器

使用[python3-ping](https://pypi.python.org/pypi/python3-ping), [python3-ping git repo](https://github.com/emamirazavi/python3-ping)

直接下载ping.py并修改verbose_ping()函数即可



