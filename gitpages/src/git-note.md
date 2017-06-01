---
title: git note
date: 2017-05-24 22:14:00
tags: [git]
---

# 使用git本地版本库实现多地同步
## 应用场景
一些笔记及音视频资料，在家和公司都可能增删文件，但是比较大，希望通过U盘作为中介同步。
<!--more-->

## 具体步骤
使用tortoise git工具。估计这就是git的常规用法之一。其实就是用u盘作为远程git库载体。

1. u盘新建 datasync.git 文件夹，并创建为本地Bare库(用于同步数据，仅支持push)
2. 公司电脑创建 datawork 文件夹，创建为本地git库（非bare）,添加文件并提交到master
3. datawork， push到Arbitrary Url，填写u盘路径如 x:\datasync.git
4. 个人电脑 git clone x:\datasync.git,为方便区分，文件夹命名为 datahome
5. 可以在 datawork 和 datahome 中增删文件，需要同步时push到 datasync.git，在另一个文件夹中pull

## 问题

- ### push时，音视频文件compress耗时较长

#### 原因
delta diff大文件耗时
#### 参考
[git-doc-gitattributes](https://git-scm.com/docs/gitattributes)
Packing objects
delta
Delta compression will not be attempted for blobs for paths with the attribute delta set to false.

[git pull without remotely compressing objects](https://stackoverflow.com/questions/7102053/git-pull-without-remotely-compressing-objects)

Git does spend a fair bit of time in zlib for some workloads, but it should not create problems on the order of minutes.

For pushing and pulling, you're probably seeing delta compression, which can be slow for large files

core.compression 0 # Didn't seem to work.
That should disable zlib compression of loose objects and objects within packfiles. It can save a little time for objects which won't compress, but you will lose the size benefits for any text files.

But it won't turn off delta compression, which is what the "compressing..." phase during push and pull is doing. And which is much more likely the cause of slowness.

pack.window 0
It sets the number of other objects git will consider when doing delta compression. Setting it low should improve your push/pull times. But you will lose the substantial benefit of delta-compression of your non-image files (and git's meta objects). So the "-delta" option above for specific files is a much better solution.

echo '*.jpg -delta' > .gitattributes
Also, consider repacking your repository, which will generate a packfile that will be re-used during push and pull.

#### 解决
使用 -delta属性禁止某些文件的delta diff操作: datawork根目录下新建 .gitattributes 文件，添加如下内容
```git-attributes
*.mp4 -delta
*.zip -delta
*.pdf -delta
```
