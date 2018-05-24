---
title: Note for CDN
date: 2017-05-24 22:14:00
tags: [note, cdn]
---

**Note for CDN**

Note for CDN learning

<!--more-->

[TOC]

# HTTP

## POST和GET的区别

https://www.zhihu.com/question/28586791

涉及概念：safe安全，Idempotent幂等，cacheable可缓存

# CDN

## COS: Cloud Object Storage 云对象存储

## CNAME：域名解析别名Canonical Name

将一个域名解析为另一个域名

https://www.zhihu.com/question/22916306

- [ ] [域名解析中A记录、CNAME、MX记录、NS记录的区别和联系](https://blog.csdn.net/benbenzhuhwp/article/details/44704319)

[CNAME和CDN](https://www.cnblogs.com/YDDMAX/p/5598196.html)

### dig

```
dig +trace www.baidu.com

1. dns获取13个根域服务器 [a-m].root-servers.net
2. 选取一个，获取gTLD（generic top-level domain)通用顶级域服务器 [?].gtld-servers.net
3. 选取一个，获取 baidu.com顶级域名服务器 dns.baidu.com / nsX.baidu.com
4. 选取一个，获取到别名服务器 cname/ns服务器 www.a.shifen.com / nsX.a.shifen.com
拿到别名后，再次请求别名解析，获取到ip地址
别名服务器可能？和源服务器是同一台服务器

1. 本机向local dns请求www.baidu.com
2. local dns向根域请求www.baidu.com，根域返回com.域的服务器IP
3. 向com.域请求www.baidu.com，com.域返回baidu.com域的服务器IP
4. 向baidu.com请求www.baidu.com，返回cname www.a.shifen.com和a.shifen.com域的服务器IP
5. 向root域请求www.a.shifen.com
6. 向com.域请求www.a.shife.com
7. 向shifen.com请求
8. 向a.shifen.com域请求
9. 拿到www.a.shifen.com的IP
10. localdns返回本机www.baidu.com cname www.a.shifen.com 以及 www.a.shifen.com的IP
```

[DNSPod帮助：CNAME记录](https://support.dnspod.cn/Kb/showarticle/tsid/32/)

［如果需要将域名指向另一个域名，再由另一个域名提供ip地址，就需要添加CNAME记录］
  最常用到CNAME的情况包括：做CDN，做企业邮局

## refer与防盗链

- [ ] [【CDN】内容分发网络中的八种防盗链技术](http://www.itts-union.com/1344.html)

HTTP refer是http表头字段，用于表明链接源站。比如www.test.com的页面的某个图片，请求图片地址是refer就是www.test.com。

当浏览器向web服务器发送请求时，会带上referer，告诉服务器此请求是从哪个链接而来，从而可进行信息的处理。

如refer白名单，当请求某一图片时，如果refer匹配到列表，才返回内容，否则拒绝。

- [ ] [阿里云CDN > 用户指南 > 访问控制设置 > 防盗链](https://help.aliyun.com/document_detail/27134.html)

功能介绍

- 防盗链功能基于 HTTP 协议支持的 Referer 机制，通过 referer 跟踪来源，对来源进行识别和判断，用户可以通过配置访问的 referer 黑白名单来对访问者身份进行识别和过滤，从而限制 CDN 资源被访问的情况
- 目前防盗链功能支持黑名单或白名单机制，访客对资源发起请求后，请求到达 CDN 节点，CDN节点会根据用户预设的防盗链黑名单或白名单，对访客的身份进行过滤，符合规则可以顺利请求到资源；若不符合规则，该访客请求被禁止，返回403响应码。

## 腾讯云cdn文档

# 简介

- [ ] [文档首页](https://cloud.tencent.com/document?)  [内容分发网络](https://cloud.tencent.com/document/product/228?)  [产品介绍](https://cloud.tencent.com/document/product/228/560?)  [产品概述](https://cloud.tencent.com/document/product/228/2939)
| 计费管理 | GetPayType, UpdatePayType                                    |
| -------- | ------------------------------------------------------------ |
| 域名管理 | AddCdnHost, OnlineHost, OfflineHost, DeleteCdnHost           |
| 配置管理 | UpdateCdnConfig, SetHttpsInfo, UpdateCdnProject              |
| 配置查询 | DescribeCdnHosts, GetHostINfoByHost, GetHostInfoById         |
| 数据查询 | DescribeCdnHostInfo, GetCdnHotsDetailStatistics, GetCdnOriginStat, GetCdnStatTop, GetCdnProvispDetailStat |
| 刷新预热 | GetCdnRefershLog, RefreshCdnUrl, RefreshCdnDir, CdnPusherV2, CdnUrlPusher, GetPushLogs, FlushOrPushOverall |
| 日志查询 | GenerateLogList, GetCdnLogList                               |
| 服务查询 | QueryCdnIp, GetCdnMonitorData                                |



- [ ] [文档首页 内容分发网络 API 文档 国内 API 文档 简介](https://cloud.tencent.com/document/product/228/15418)
## 新增加速域名

[文档首页   内容分发网络   API 文档   国内 API 文档   域名管理   新增加速域名](https://cloud.tencent.com/document/product/228/1406)

AddCdnHost  cdn.api.qcloud.com

参数：

host, projectId, hostType, origin, backupOrigin, serviceType, fullUrl, fwdHost, cache, chcheMode, refer, accessIp

域名， 项目id，接入方式，源站， 备份源，加速类型，url参数过滤，回源域名，缓存时间，缓存模式，防盗链，ip安全

[新增加速域名调用示例](https://cloud.tencent.com/document/product/228/1734)

# LINUX操作

```
nslookup www.baidu.com
dig +trace www.baidu.com
```



