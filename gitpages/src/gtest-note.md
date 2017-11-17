---
title: gtest note
date: 2017-05-24 22:14:00
tags: [tag1, tag2]
---

**Note for Google Test (gtest)**

Note for Google Test (gtest)

<!--more-->

[TOC]

# Googletest on github

Repo:

[google](https://github.com/google)/[**googletest**](https://github.com/google/googletest)

##[Google Test Primer](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md) 

### Why gtest?

基于xUniti架构。设计理念：

1. 测试独立、可重复 (independent, repeatable)
2. 条理清晰 (well organized)
3. 平台(OS/compiler)无关，可重用 (portable, reusable)
4. 带错执行（某个test失败后继续执行后续test）
5. 简化程序员工作，自动跟踪test实例。
6. 快速。

### Setting up

git clone and compile.

easy way: `vcpkg install gtest:x64-windows`，手动链接gtest.lib / gmock.lib (`installed\x64-windows\lib\manual-link`)

### Structure

tests -> test cases -> test fixture

### Basic Concepts & Assertions

assertions on condition:

- success
- nonfatal failure: `EXPCET_*`
- fatal failure: abort current function, `ASSERT_*`
- 通过`<<`追加自定义错误信息，unicode自动转换为utf-8

`ASSERT_*` 立即返回，不会执行后续清理代码，可能导致资源泄露，以及堆校验错误。某些情况下可以不管。

#### Basic Assertions 布尔

- `ASSERT_TRUE/FALSE(condition)`
- `EXPECT_TRUE/FALSE(condition)`


#### Binary Comparison 数值

对两个变量进行比较，失败时打印val1及val2。

- `ASSERT_EQ(val1,val2)`, `EXPECT_EQ(val1,val2)`
- EQ == , NE != , LT < , LE <= , GT > , GE >=


注意事项：

- 对自定义类型，建议使用ASSERT_*，会同时打印相应操作符
- 每个参数只会被计算一次。
- 对指针来说，ASSERT_EQ 比较地址，而非指针内容。

#### String Comparison 字串

对C字串进行比较。string对象使用*_EQ/NE函数

- `ASSERT/EXPECT_STREQ/STRNE/STRCASEEQ/STRCASENE`

case表示忽略大小写。支持wchar_t*，失败后使用utf-8打印错误信息。

### Simple Tests

`TEST(TestCaseName, TestName){...}`

参数名不能含有下划线。gtest通过TestCaseName组织测试结果。

```
// test case 'FactorialTest' with two tests: HandlesZeroInput, HandlesPositiveInput
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(1, Factorial(0));
}
TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));
  ...
}
```

### Test Fixtures

`TEST_F(FixtureClassName, TestName)`

对于不同的测试，使用同一个类的不同实例，互不影响。具体流程：

- 创建新的fixture实例，并调用SetUp()初始化
- 运行测试
- 调用TearDown()清理，并销毁fixture实例。

```
// fixture class
class QueueTest : public ::testing::Test {
 protected: // protected or public, 子类可访问
  virtual void SetUp() {
    q1_.Enqueue(1);
    q2_.Enqueue(2);
  }

  // virtual void TearDown() {} // 不需要可以省略
  
  // 变量可再test中直接使用
  Queue<int> q0_;
  Queue<int> q1_;
};

// test fixture
TEST_F(QueueTest, IsEmptyInitially) {
  EXPECT_EQ(0, q0_.size());
}

TEST_F(QueueTest, DequeueWorks) {
  int* n = q0_.Dequeue();
  EXPECT_EQ(NULL, n);	// if fail, continue

  n = q1_.Dequeue();
  ASSERT_TRUE(n != NULL); // if fail, exit, or dereference will crash
  EXPECT_EQ(1, *n);
  delete n;
}
```

### Invoking the Tests 调用测试

`TEST()`和`TEST_F()`会自动注册测试用例。调用`RUN_ALL_TESTS()`运行所有测试（只能调用一次），成功返回0。

```
test loop:
  保存gtest flag
  创建fixture对象,SetUp
  运行test
  TearDown，销毁fixture
  恢复gtest lag     
```

### main()示例

```
#include "this/package/foo.h"
#include "gtest/gtest.h"

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  FooTest() { ... }
  virtual ~FooTest() { ... }
  virtual void SetUp()  { ... }
  virtual void TearDown()  { ... }
  // members used in TEST_F
};

TEST_F(FooTest, MethodBarDoesAbc)  { ... }
TEST_F(FooTest, DoesXyz)  { ... }
...

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

### note on VC++

由于VC的bug/特性，导致lib库中的test不能被正确执行。解决方案：

- 程序代码中显示调用库中的任意函数（可以为空）
- 不要将test放在在lib库中

### what's next

[samples](Samples.md)
[AdvancedGuide](AdvancedGuide.md)

## [Google Mock documentation](https://github.com/google/googletest/blob/master/googlemock/README.md)

## [googletest/README.md](https://github.com/google/googletest/blob/master/googletest/README.md) 



