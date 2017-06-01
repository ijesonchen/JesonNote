---
title: python note
date: 2017-05-24 22:14:00
tags: [python]
---

python笔记
<!--more-->

# os.scandir 快速遍历目录
imort os
遍历目录
os.listdir() 较慢
for ent in os.scandir() 较快，返回DirEntry
	ent.is_dir()
	ent.is_file()
	ent.name
	ent.path
	ent.stat() 可以获取文件时间
	
# multiprocessing.Pool 进程池

from multiprocessing import Pool

def f(x):
    return x*x

if __name__ == '__main__':
	x = [1, 2, 3]
    with Pool(5) as p:
        print(p.map(f, x))
		
# zipfile 压缩打包
import zipfile
from zipfile imort *
with ZipFile(zipname, 'w') as zipFile:
	for x in os.scandir(dirname):
		fullname = base + '\\' + x.name
		zipFile.write(fullname, arcname = x.name, compress_type = zipfile.ZIP_DEFAULTED)

