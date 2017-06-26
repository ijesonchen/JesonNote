---
title: how to use markdown
date: 2017-06-26 14:24:00
tags: [markdown, how-to]
---

介绍如何使用markdown的一些语法、工具和技巧

<!--more-->

[TOC]

# 语法
## 目录
需要显示目录的地方直接插入[TOC]标记即可（前后可能需要有空行）
编辑器支持可能存在bug，haroopad会忽略一级标题。

# 工具
update: Mon, Jun 26, 2017 2:26 PM

## haroopad 0.13.1 2015.03
支持同时编辑+预览界面（两个界面）
有些小bug，似乎已经停止更新
已知bug：
- [TOC]不显示一级标题
- 编辑界面输入的中文字符不显示（修改default.css添加字体后解决）
- 某些情况下，偏好设置界面中，中文字符无法显示

## typora 0.9.29
所见即所得，直接在预览界面编辑，更新快速

# 插件
## mermaid 流程图
[mermaid](http://knsv.github.io/mermaid/)：Generation of diagrams and flowcharts from text in a similar manner as markdown.
似乎haroopad同一个文件中同事汇制多张图偶尔有预览bug，多张图会叠加，有时会自行恢复。
typora不存在该问题。

### flowchart
~~~mermaid
graph LR;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
~~~


### sequence diagram
~~~mermaid
sequenceDiagram
    participant Alice
    participant Bob
    Alice->John: Hello John, how are you?
    loop Healthcheck
        John->John: Fight against hypochondria
    end
    Note right of John: Rational thoughts <br/>prevail...
    John-->Alice: Great!
    John->Bob: How about you?
    Bob-->John: Jolly good!
~~~

### gantt diagram
~~~mermaid
gantt
        dateFormat  YYYY-MM-DD
        title Adding GANTT diagram functionality to mermaid
        section A section
        Completed task            :done,    des1, 2014-01-06,2014-01-08
        Active task               :active,  des2, 2014-01-09, 3d
        Future task               :         des3, after des2, 5d
        Future task2               :         des4, after des3, 5d
        section Critical tasks
        Completed task in the critical line :crit, done, 2014-01-06,24h
        Implement parser and jison          :crit, done, after des1, 2d
        Create tests for parser             :crit, active, 3d
        Future task in critical line        :crit, 5d
        Create tests for renderer           :2d
        Add to mermaid                      :1d
~~~