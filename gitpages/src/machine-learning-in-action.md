---
title: Note for Machine Learning In Action
date: 2017-05-24 00:00 :00
tags: [note, machine learning]
---

**Note for Machine Learning In Action**

机器学习实战代码笔记

<!--more-->

英文免费电子书 https://www.manning.com/books/machine-learning-in-action 。可以找到pdf下载。有中文版《机器学习实战》，2013年出版。采用的python版本是python2。涉及到到多个python包，python3可以pip install matplotlib，会同时安装numpy。IDE可以考虑使用IntelliJ IDEA+python插件，或者pycharm，或者vscode。

[TOC]

# 分类

## KNN

k-Nearest Neighbors

```
原理
0. 训练数据：样本特征-分类关系数据.无训练流程
1. 计算输入和每个训练样本的距离，取前k个（一般20以内，最近邻居）样本
2. k个样本中，出现频率最高的分类，即为输入的分类。
重点：样本间距离的计算。最简单，使用欧式几何距离。
```



# build-in

函数

```
sorted
```



# numpy

# ndarray

函数

```
tile
```



N维数组

```
成员
min
max
shape：行列数组
dtype
size
array

方法：
sum
argsort
```









