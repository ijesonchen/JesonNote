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

# 索引的使用

同时进行Query和Sort操作时，使用使用sort所含字段的索引。如果有多个符合条件的索引，优先使用先创建的索引（MongoDB 3.4.5)。

但是，如果sort没有直接索引，但是query使用索引的结果自然符合sort顺序时，使用sort索引（参考[Use Indexes to Sort Query Results](https://docs.mongodb.com/manual/tutorial/sort-results-with-indexes/))

测试索引：

```
s1, e-1
e1, s1
p1, t-1
e1, s1, p1, t-1
p1, t-1, e1, s1
```



## 1) 同时Query和Sort，使用第一个符合要求的Sort索引

query：e,s	sort：p,t

索引建立顺序ptes,pt，则选中ptes；建立顺序pt，ptes，则选中pt

先建立ptes，再建立pt时:

```
选中：ptes 可以通过hint使用pt
拒绝：espt,es,se，主要在检索阶段，试图匹配不同范围条件。
阶段SORT,GEN,FETCH,OR：
se, espt
es, espt
espt, espt
se, es
es, es
espt, es
```

查询

```
db.getCollection('TRecord').find({ "$or" : [ 
 { "endTime" : ISODate("1970-01-01T08:00:00.000+08:00"), 
	"startTime" : { "$lte" : ISODate("2018-04-18T00:00:00.000+08:00") } }, 
 { "endTime" : { "$gt" : ISODate("1970-01-01T08:00:00.000+08:00") } } 
 ]})
 .sort({ "priority" : 1, "tmAssign" : -1 })
 .explain()
```



## 2) Sort多个使用第一个符合要求的索引

sort: p,t 

索引建立顺序ptes,pt，则选中ptes；建立顺序pt，ptes，则选中pt

先建立ptes，再建立pt时:

```
选中：ptes 可以通过hint使用pt
拒绝：无
```

查询

```
db.getCollection('TRecord').find({ })
 .sort({ "priority" : 1, "tmAssign" : -1 })
 .explain()
```



## 3) Query多个索引可用

query: es

```
选中：es, espt 可以通过hint只使用其中一个
拒绝：无
```

查询

```
db.getCollection('TRecord').find({ "$or" : [ 
 { "endTime" : ISODate("1970-01-01T08:00:00.000+08:00"), 
	"startTime" : { "$lte" : ISODate("2018-04-18T00:00:00.000+08:00") } }, 
 { "endTime" : { "$gt" : ISODate("1970-01-01T08:00:00.000+08:00") } } 
 ]})
 .explain()
```

## 4) 调整索引建立顺序

调整索引顺序后，先pt后ptes，含有sort的查询选择pt索引

# 一个性能降低案例分析

April 21, 2017 11:15 PM

观察到Mongodb运行一段时间后性能急剧降低.100W数据操作周期（包含较多其他操作）大约会由最初的半小时，到一个小时，甚至后来的4-5个小时才能完成。(后来观察发现是由于运维人员不当操作导致，对较大表进行无索引查询，但后续优化思路仍有价值。可以使用MongoChef等查看数据库当前慢操作。)
今天分析数据库发现，主表T1含有数据1.7G条，索引大小超过100G，另外一张数据量较大的表T2数据量超过1.5G,索引文件10G+,这两张表是使用最频繁的两张，几乎每条记录都会产生5-10次操作。此外还有几张常用的表数据量也比较大。
数据库服务器内存容量为128G，未运行其他服务。
推测是因为数据库索引文件过大，导致无法将所需索引全部载入到内存，导致查询、更新、删除操作效率降低。
将T1，T2删除掉后数据操作恢复到较好状态。因为数据流量较小，等周一流量较大时做详细测试。

优化思路：保持数据库规模处于一个较小的水平。

- 全面整理所需要的操作，删除不必要的索引，清理复合索引
- 使用TTL（Time To Live）。对于可过期记录，增加插入时间，并设置索引expire_after_seconds属性，过期自动删除
- 专表专用，空间换时间。对于不重要的某些辅助信息，使用专门的表记录，并定期清理
- 自动分表归档
- 数据量实在过大（每天一亿条以上规模），则所有记录处理过程中携带一个时间信息，按日期分表。



# Manual

[The MongoDB Manual](https://docs.mongodb.com/manual/)

current version is 3.6 （2018.04.16）

## [Indexes](https://docs.mongodb.com/manual/indexes/)

### [Indexing Strategies](https://docs.mongodb.com/manual/applications/indexes/)

The best indexes for your application must take a number of factors into account, including the kinds of queries you expect, the ratio of reads to writes, and the amount of free memory on your system.

程序最佳的索引需要考虑一些因素，包括期望的查询类型，读写比例，以及系统空闲内存总量。

When developing your indexing strategy you should have a deep understanding of your application’s queries. Before you build indexes, map out the types of queries you will run so that you can build indexes that reference those fields. Indexes come with a performance cost, but are more than worth the cost for frequent queries on large data set. Consider the relative frequency of each query in the application and whether the query justifies an index.

当制定索引策略是，需要对程序要执行的查询有非常深的理解。建立索引之前，首先规划将要用到的查询类型以便对相关字段建立索引。索引会降低性能，但是在大数据集的高频查询下是值得的。考虑程序的每一个查询的频率并决定该查询是否需要索引。

The best overall strategy for designing indexes is to profile a variety of index configurations with data sets similar to the ones you’ll be running in production to see which configurations perform best. Inspect the current indexes created for your collections to ensure they are supporting your current and planned queries. If an index is no longer used, drop the index.

设计索引的最好的总体策略是，在和生产系统类似的数据集上，对多种索引策略进行性能评估，并选取最好的。检查数据集当前建立的索引并确保他们支持当前和将来的查询。及时删除不需要的索引。

Generally, MongoDB only uses *one* index to fulfill most queries. However, each clause of an [`$or`](https://docs.mongodb.com/manual/reference/operator/query/or/#op._S_or) query may use a different index, and starting in 2.6, MongoDB can use an [intersection](https://docs.mongodb.com/manual/core/index-intersection/) of multiple indexes.

一般来说，MongoDB仅使用一个索引来满足大部分查询。但是，一个含有$or的查询字句可能使用不同的索引，并且自从2.6开始，MongoDB可以使用多个索引的交叠（ [intersection](https://docs.mongodb.com/manual/core/index-intersection/) ）部分。

The following documents introduce indexing strategies:

- [Create Indexes to Support Your Queries](https://docs.mongodb.com/manual/tutorial/create-indexes-to-support-queries/)

  An index supports a query when the index contains all the fields scanned by the query. Creating indexes that supports queries results in greatly increased query performance.

- [Use Indexes to Sort Query Results](https://docs.mongodb.com/manual/tutorial/sort-results-with-indexes/)

  To support efficient queries, use the strategies here when you specify the sequential order and sort order of index fields.

- [Ensure Indexes Fit in RAM](https://docs.mongodb.com/manual/tutorial/ensure-indexes-fit-ram/)

  When your index fits in RAM, the system can avoid reading the index from disk and you get the fastest processing.

- [Create Queries that Ensure Selectivity](https://docs.mongodb.com/manual/tutorial/create-queries-that-ensure-selectivity/)

  Selectivity is the ability of a query to narrow results using the index. Selectivity allows MongoDB to use the index for a larger portion of the work associated with fulfilling the query.

### [Use Indexes to Sort Query Results](https://docs.mongodb.com/manual/tutorial/sort-results-with-indexes/)

按照先sort字段，后query字段顺序建立复合索引。

On this page

- [Sort with a Single Field Index](https://docs.mongodb.com/manual/tutorial/sort-results-with-indexes/#sort-with-a-single-field-index)
- [Sort on Multiple Fields](https://docs.mongodb.com/manual/tutorial/sort-results-with-indexes/#sort-on-multiple-fields)
- [Index Use and Collation](https://docs.mongodb.com/manual/tutorial/sort-results-with-indexes/#index-use-and-collation)

In MongoDB, sort operations can obtain the sort order by retrieving documents based on the ordering in an index. If the query planner cannot obtain the sort order from an index, it will sort the results in memory. Sort operations that use an index often have better performance than those that do not use an index. In addition, sort operations that do *not* use an index will abort when they use 32 megabytes of memory.

MongoDB排序操作可以利用索引的顺序排序数据documents。如果查询计划器query planer无法从索引中获取顺序，则会在内存中排序。使用索引的排序操作通常比没有索引的排序具有更好的性能。同时，不使用索引的排序操作使用内存超过32M时将会终止。

NOTE

As a result of changes to sorting behavior on array fields in MongoDB 3.6, when sorting on an array indexed with a [multikey index](https://docs.mongodb.com/manual/core/index-multikey/) the query plan includes a blocking SORT stage. The new sorting behavior may negatively impact performance.
MongoDB 3.6在数组字段的排序行为的改变导致，当在多键索引[multikey index](https://docs.mongodb.com/manual/core/index-multikey/)（index on field on array value, 数组字段上的索引）上排序时，查询计划会包含一个阻塞的排序阶段。这个新的排序行为将会降低性能。

In a blocking SORT, all input must be consumed by the sort step before it can produce output. In a non-blocking, or *indexed* sort, the sort step scans the index to produce results in the requested order.
在阻塞排序中，产生输出前排序步骤必须处理完所有输入。在非阻塞或有索引的排序中，排序步骤扫描索引即可产生所需要的序列。

### Sort with a Single Field Index

If an ascending or a descending index is on a single field, the sort operation on the field can be in either direction.
如果一个字段具有升序或降序索引，该字段的排序操作可以任意顺序。

For example, create an ascending index on the field `a` for a collection `records`:

```
db.records.createIndex( { a: 1 } )
```

This index can support an ascending sort on `a`:

```
db.records.find().sort( { a: 1 } )
```

The index can also support the following descending sort on `a` by traversing the index in reverse order:

```
db.records.find().sort( { a: -1 } )
```

### Sort on Multiple Fields

Create a [compound index](https://docs.mongodb.com/manual/core/index-compound/#index-type-compound) to support sorting on multiple fields.
创建复合索引以支持在多个字段上的排序。

You can specify a sort on all the keys of the index or on a subset; however, the sort keys must be listed in the *same order* as they appear in the index. For example, an index key pattern `{ a: 1, b: 1 }` can support a sort on `{ a: 1, b: 1 }` but *not* on `{ b: 1, a: 1 }`.
可以指定索引上的所有或部分键进行排序。但是，排序的键必须和索引中的顺序相同。例如，索引`{ a: 1, b: 1 }`可以支持`{ a: 1, b: 1 }`的排序，但不支持`{ b: 1, a: 1 }`。

For a query to use a compound index for a sort, the specified sort direction for all keys in the [`cursor.sort()`](https://docs.mongodb.com/manual/reference/method/cursor.sort/#cursor.sort) document must match the index key pattern *or* match the inverse of the index key pattern. For example, an index key pattern `{ a: 1, b: -1 }` can support a sort on `{ a: 1, b: -1 }` and `{ a:-1, b: 1 }` but **not** on `{ a: -1, b: -1 }` or `{a: 1, b: 1}`.
为了使一个查询可以使用复合索引，指定的所有字段的排序方向必须和索引方向中的相同或相反。例如索引`{ a: 1, b: -1 }`可以支持`{ a: 1, b: -1 }`和`{ a:-1, b: 1 }`的排序，但不支持`{ a: -1, b: -1 }`和`{a: 1, b: 1}`。

### Sort and Index Prefix
排序和索引前缀

If the sort keys correspond to the index keys or an index *prefix*, MongoDB can use the index to sort the query results. A *prefix* of a compound index is a subset that consists of one or more keys at the start of the index key pattern.
如果排序字段是一个索引的前缀，MongoDB可以使用该索引排序查询结果。复合索引的前缀是由从索引字段开始的一个或多个字段组成的子集。

For example, create a compound index on the `data` collection:
例如，在`data`上创建的复合索引：
```
db.data.createIndex( { a:1, b: 1, c: 1, d: 1 } )
```

Then, the following are prefixes for that index:
那么，该索引的前缀是：

```
{ a: 1 }
{ a: 1, b: 1 }
{ a: 1, b: 1, c: 1 }
```

The following query and sort operations use the index prefixes to sort the results. These operations do not need to sort the result set in memory.
以下查询和排序操作使用索引前缀排序结果。这些操作不需要再内存中重新排序结果。

| Example                                                    | Index Prefix           |
| ---------------------------------------------------------- | ---------------------- |
| `db.data.find().sort( { a: 1 } )`                          | `{ a: 1 }`             |
| `db.data.find().sort( { a: -1 } )`                         | `{ a: 1 }`             |
| `db.data.find().sort( { a: 1, b: 1 } )`                    | `{ a: 1, b: 1 }`       |
| `db.data.find().sort( { a: -1, b: -1 } )`                  | `{ a: 1, b: 1 }`       |
| `db.data.find().sort( { a: 1, b: 1, c: 1 } )`              | `{ a: 1, b: 1, c: 1 }` |
| `db.data.find( { a: { $gt: 4 } } ).sort( { a: 1, b: 1 } )` | `{ a: 1, b: 1 }`       |

Consider the following example in which the prefix keys of the index appear in both the query predicate and the sort:
考虑如下示例，索引的前缀字段在查询和排序字句中都出现：

```
db.data.find( { a: { $gt: 4 } } ).sort( { a: 1, b: 1 } )
```

In such cases, MongoDB can use the index to retrieve the documents in order specified by the sort. As the example shows, the index prefix in the query predicate can be different from the prefix in the sort.
这种情况下，MongoDB先使用索引排序，然后按照排序结果中的顺序获取结果集。该示例表明，查询字句和排序字句中的查询前缀具有不同的效果。

### Sort and Non-prefix Subset of an Index

An index can support sort operations on a non-prefix subset of the index key pattern. To do so, the query must include **equality** conditions on all the prefix keys that precede the sort keys.
索引可以支持非索引前缀子集上的排序操作。为了实现这一点，查询条件需要满足以下等效条件，即包含可以生成排序字段的所有前缀字段。

For example, the collection `data` has the following index:
例如，数据集`data`上建立索引：

```
{ a: 1, b: 1, c: 1, d: 1 }
```

The following operations can use the index to get the sort order:
如下操作可以使用索引产生排序序列：
| Example                                                   | Index Prefix            |
| --------------------------------------------------------- | ----------------------- |
| `db.data.find( { a: 5 } ).sort( { b: 1, c: 1 } )`         | `{ a: 1 , b: 1, c: 1 }` |
| `db.data.find( { b: 3, a: 4 } ).sort( { c: 1 } )`         | `{ a: 1, b: 1, c: 1 }`  |
| `db.data.find( { a: 5, b: { $lt: 3} } ).sort( { b: 1 } )` | `{ a: 1, b: 1 }`        |

As the last operation shows, only the index fields *preceding* the sort subset must have the equality conditions in the query document; the other index fields may specify other conditions.
最后一个操作显示，只要查询结果使用索引产生了排序子集必定是等效条件，其他的索引字段显示了不同的情况。（前2个条件为精确值，按照索引结果自然产生满足sort条件的序列。第三个查询虽然使用了范围，但是a为准确值，b为范围，按照索引结果仍然产生了满足sort条件的序列。）

If the query does **not** specify an equality condition on an index prefix that precedes or overlaps with the sort specification, the operation will **not** efficiently use the index. For example, the following operations specify a sort document of `{ c: 1 }`, but the query documents do not contain equality matches on the preceding index fields `a` and `b`:
如果查询没有指定索引前缀的等效条件，即结果和排序要求不相同或重叠，操作将不能有效使用索引。例如，如下操作要求按`{ c: 1 }`排序，但是查询结果不包含含有a,b的前缀索引的等效条件。

```
db.data.find( { a: { $gt: 2 } } ).sort( { c: 1 } )
db.data.find( { c: 5 } ).sort( { c: 1 } )
```

These operations **will not** efficiently use the index `{ a: 1, b: 1, c: 1, d: 1 }` and may not even use the index to retrieve the documents.
这些操作不会有效使用索引`{ a: 1, b: 1, c: 1, d: 1 }` ，甚至都会使用索引获取结果。

### Index Use and Collation
指定索引中所用的排序规则（如字符串语种相关排序规则）

To use an index for string comparisons, an operation must also specify the same collation. That is, an index with a collation cannot support an operation that performs string comparisons on the indexed fields if the operation specifies a different collation.

For example, the collection `myColl` has an index on a string field `category` with the collation locale `"fr"`.
例如，数据集`myColl`指定了索引字段`category`使用`"fr"`规则。

```
db.myColl.createIndex( { category: 1 }, { collation: { locale: "fr" } } )
```

The following query operation, which specifies the same collation as the index, can use the index:
如下指定了和索引相同规则的查询可以使用索引。

```
db.myColl.find( { category: "cafe" } ).collation( { locale: "fr" } )
```

However, the following query operation, which by default uses the “simple” binary collator, cannot use the index:
但是，如下查询使用默认的二进制排序规则，不会使用索引。

```
db.myColl.find( { category: "cafe" } )
```

For a compound index where the index prefix keys are not strings, arrays, and embedded documents, an operation that specifies a different collation can still use the index to support comparisons on the index prefix keys.
对于复合索引，如果前缀字段不是字符串，数据，或者内嵌文档，指定了不同规则的操作仍然可以使用前缀索引部分（但是对于指定了比较规则的字符串部分，除非查询指定，否则默认使用简单的二进制比较，不会使用完整索引，但是前缀部分索引仍然可以使用）。

For example, the collection `myColl` has a compound index on the numeric fields `score` and `price` and the string field `category`; the index is created with the collation locale `"fr"` for string comparisons:

```
db.myColl.createIndex(
   { score: 1, price: 1, category: 1 },
   { collation: { locale: "fr" } } )
```

The following operations, which use `"simple"` binary collation for string comparisons, can use the index:

```
db.myColl.find( { score: 5 } ).sort( { price: 1 } )
db.myColl.find( { score: 5, price: { $gt: NumberDecimal( "10" ) } } ).sort( { price: 1 } )
```

The following operation, which uses `"simple"` binary collation for string comparisons on the indexed `category` field, can use the index to fulfill only the `score: 5` portion of the query:

```
db.myColl.find( { score: 5, category: "cafe" } )
```

