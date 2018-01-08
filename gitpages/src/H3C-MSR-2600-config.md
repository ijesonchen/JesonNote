---
title: template for gitpages
date: 2018-01-08 22:14:00
tags: [H3C, 路由器配置]
---


# H3C MSR 2600 路由双线配置

两个宽带网接入网络，内部流量根据需要通过不同端口接入外部网络。
<!--more-->

H3C MSR 2600-10-X1

GE0: WAN 电信PPPoE

GE1: WAN 联通固定IP

GE2-8：VLAN1， 192.168.1.1 DHCP 51-199

GE9: VLAN9, 192.168.9.1 DHCP 2-254， 联通出口

[TOC]



## 重置/恢复出厂设置

按住reset键3秒以上，等待5分钟左右重置并启动完成

Compiled Date       : Apr 14 2017
CPU ID              : 0x8
CPU L1 Cache        : 32KB
CPU L2 Cache        : 256KB
Memory Type         : DDR3 SDRAM
Memory Size         : 1024MB
Flash Size          : 256MB
CPLD Version        : 1.0
PCB Version         : 2.0

重置并加载镜像 ，启动，自检并测试，自动配置

0:30：System image is starting...

1:20: test

1:40: test passed

2:40: Startup configuration file does not exist.Performing automatic configuration.

4:00: Performing automatic configuration.





## vlan划分

连接GE2，登录 192.168.0.1

网络设置，LAN配置，修改valn1配置。重新启用本地网卡，分配到新的IP地址 192.168.1.51。

进入192.168.1.1，LAN配置，VLAN配置，添加VLAN2及端口。

另一台电脑连接GE9，分配到IP地址192.168.2.2

测试可以实现 1.51和2.2的互ping

## WAN口配置

测试时，两WAN口使用固定ip配置：

GE0: 10.0.0.10

GE1: 10.0.1.10

## VPN配置

```
按下述顺序配置好之后，windows自带客户端无法连接VPN，具体原因不详。
官方提供的L2TP over IPsec方法比较复杂，需要命令行配置，登陆需要使用iNode客户端。
先配置L2TP，可以登录之后再配置IPsec可能能正常连接。具体步骤不详，但可以实现。Windows系统需要修改注册表，禁用IpSec。
```

### PPPoE服务器

修改，应用于：GE0/GE1

### 用户管理

添加用户，PPP服务，设置密码

### IPsec VPN

对每个接口设置不同ipsec名称，组网方式（点到多点），预共享密钥

高级配置，协商模式，野蛮模式

### L2TP服务器

启用L2TP服务器端，添加

用户组号，隧道验证：禁用，PPP认证方式:CHAP，server配置：

192.168.12.1/255.255.255.0/192.168.12.2-192.168.12.254



## 路由策略

```
sys
// 联通流量原路返回
access-list advanced 3000
rule 5 permit ip source [联通IP] 0
quit

policy-based-route 1 permit node 1
if-match acl 3000
apply next-hop [联通网关]
quit

ip local policy-based-route 1

// 13段走联通
access-list advanced 3001
rule 5  deny ip source 192.168.13.0 0.0.0.255 destination 192.168.13.0 0.0.0.255
rule 10 deny ip source 192.168.13.0 0.0.0.255 destination 192.168.1.0  0.0.0.255
rule 15 deny ip source 192.168.13.0 0.0.0.255 destination 192.168.9.0  0.0.0.255
rule 20 deny ip source 192.168.13.0 0.0.0.255 destination 192.168.11.0 0.0.0.255
rule 25 permit ip source 192.168.13.0 0.0.0.255
quit

policy-based-route 2 permit node 1
if-match acl 3001
apply next-hop [联通网关]
quit

int vlan 3
ip policy-based-route 2
quit

// 9段走联通
access-list advanced 3002
rule 5  deny ip source 192.168.9.0 0.0.0.255 destination 192.168.9.0 0.0.0.255
rule 10 deny ip source 192.168.9.0 0.0.0.255 destination 192.168.1.0  0.0.0.255
rule 15 deny ip source 192.168.9.0 0.0.0.255 destination 192.168.13.0  0.0.0.255
rule 20 deny ip source 192.168.9.0 0.0.0.255 destination 192.168.11.0 0.0.0.255
rule 25 permit ip source 192.168.9.0 0.0.0.255
quit

policy-based-route 3 permit node 1
if-match acl 3002
apply next-hop [联通网关]
quit

int vlan 9
ip policy-based-route 3
quit

// 默认走电信
ip route-static 0.0.0.0 0 Dialer0 preference 50
```

## 静态IP

```
sys
dis dhcp server pool // 显示IP地址池
dhcp server ip-pool [pool name]
undo static-bind ip-address x.x.x.x // 删除静态一条地址表
static-bind ip-address x.x.x.x mask x.x.x.x hardware-address x-x-x // 绑定ip-mac
```





## 问题

### 1. L2TP over IPsec配置流程

官方仅提供了命令行方式的配置，比较复杂。

MSR 930及MSR 2600都实现了通过界面配置的L2TP over IPsec。具体流程不确定。因为一开始配置好L2TP后可以拨号，加入IPsec后拨号失败，后来改了某些设置又好了。

### 2. 电信无法实现多个VLAN访问web

内部划分为多个VLAN，默认走电信接口时。仅1.x可以访问web，其他网段无法正常访问web（仅可访问国外站点，推测是因为开通了国际精品网业务，有国外流量加速）。

即使重置设置，不配置网络路由策略，VPN等，仅划分多个VLAN，情况依旧。

但是 

1. VPN接入后11.x可以正常上网，流量走GE0电信。
2. GE0使用联通口，可以内部多个VLAN都可以上网。
3. 内部划分多个VLAN后，配置路由策略，除了1.x之外，其他VLAN走联通口，可以正常上网。

根据客服意见，具体原因需要做端口镜像抓包诊断。未处理。