---
title: 如何利用循环代替递归以防止栈溢出(译)
date: 2017-06-18 20:05
tags: [programming]
---

如何利用循环代替递归以防止栈溢出(译)
钱吉
How to replace recursive functions using stack and while-loop to avoid the stack-overflow
Woong Gyu La, 1 Jul 2015
摘要：我们经常会用到递归函数，但是如果递归深度太大时，往往导致栈溢出。而递归深度往往不太容易把握，所以比较安全一点的做法就是：用循环代替递归。文章最后的原文里面讲了如何用10步实现这个过程，相当精彩。本文翻译了这篇文章，并加了自己的一点注释和理解。
<!--more-->

译文：
[如何利用循环代替递归以防止栈溢出(译)](http://www.cnblogs.com/wb-DarkHorse/archive/2013/11/15/3284228.html)
原文：
[How to replace recursive functions using stack and while-loop to avoid the stack-overflow](https://www.codeproject.com/Articles/418776/How-to-replace-recursive-functions-using-stack-and)


目录

[TOC]

# 译文

摘要：我们经常会用到递归函数，但是如果递归深度太大时，往往导致栈溢出。而递归深度往往不太容易把握，所以比较安全一点的做法就是：用循环代替递归。文章最后的原文里面讲了如何用10步实现这个过程，相当精彩。本文翻译了这篇文章，并加了自己的一点注释和理解。


## 1. 简介
一般我们在进行排序(比如归并排序)或者树操作时会用到递归函数。但是如果递归深度达到一定程度以后，就会出现意想不到的结果比如堆栈溢出。虽然有很多有经验的开发者都知道了如何用循环函数或者栈加while循环来代替递归函数，以防止栈溢出，但我还是想分享一下这些方法，这或许会对初学者有很大的帮助。

## 2. 模拟函数的目的
如果你正在使用递归函数，并且没有控制递归调用，而栈资源又比较有限，调用层次过深的时候就可能导致栈溢出/堆冲突。模拟函数的目的就是在堆中开辟区域来模拟栈的行为，这样你就能控制内存分配和流处理，从而避免栈溢出。如果能用循环函数来代替效果会更好，这是一个比较需要时间和经验来处理的事情，出于这些原因，这篇文章为初学者提供了一个简单的参考，怎样使用循环函数来替代递归函数，以防止栈溢出？

## 3. 递归和模拟函数的优缺点
### 递归函数：
　　优点：算法比较直观。可以参考文章后面的例子
　　缺点：可能导致栈溢出或者堆冲突

　　你可以试试执行下面两个函数(后面的一个例子)，IsEvenNumber(递归实现)和IsEvenNumber(模拟实现)，他们在头文件"MutualRecursion.h"中声明。你可以将传入参数设定为10000，像下面这样：
```language
#include "MutualRecursion.h" 
bool result = IsEvenNumberLoop(10000); // 成功返回
bool result2 = IsEvenNumber(10000);     // 会发生堆栈溢出
```

 有些人可能会问：如果我增加栈的容量不就可以避免栈溢出吗？好吧，这只是暂时的解决问题的办法，如果调用层次越来越深，很有可能会再次发生溢出。

### 模拟函数
　　优点：能避免栈溢出或者堆冲突错误，能对过程和内存进行更好的控制
　　缺点：算法不是太直观，代码难以维护
  
## 4. 用栈和循环代替递归的10个步骤

### 1. 定义帧结构

1. 定义一个新的结构体Snapshot，用于保存递归结构中的一些数据和状态信息
2. 在Snapshot内部需要包含的变量有以下几种：

　　A 一般当递归函数调用自身时，函数参数会发生变化。所以你需要包含变化的参数，引用除外。比如下面的例子中，参数n应该包含在结构体中，而retVal不需要。
```language
void SomeFunc(int n, int &retVal);
```
　　B 阶段性变量"stage"(通常是一个用来转换到另一个处理分支的整形变量)，详见第六条规则
　　C 函数调用返回以后还需要继续使用的局部变量(一般在二分递归和嵌套递归中很常见)

代码：
```language
// Recursive Function "First rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}


// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
    // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    ...
}
```

### 2. 定义临时变量存储递归返回值
在函数的开头创建一个局部变量，这个值扮演了递归函数的返回函数角色。它相当于为每次递归调用保存一个临时值，因为C++函数只能有一种返回类型，如果递归函数的返回类型是void，你可以忽略这个局部变量。如果有缺省的返回值，就应该用缺省值初始化这个局部变量。
```language
// Recursive Function "Second rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    ...
    // (Second rule)
    return retVal;
}
```

### 3. 创建一个栈用于保存“Snapshot”结构体类型变量
```language
// Recursive Function "Third rule" example

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    ...
    // (Second rule)
    return retVal;
}
```

### 4. 创建一个新的”Snapshot”实例，然后将其中的参数等初始化，并将“Snapshot”实例压入栈

```language
// Recursive Function "Fourth rule" example

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    // (Third rule)
    stack<SnapShotStruct> snapshotStack;

    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage

    snapshotStack.push(currentSnapshot);

    ...
    // (Second rule)
    return retVal;
}
```

### 5. 写一个while循环，使其不断执行直到栈为空。在while循环的每一次迭代过程中，弹出”Snapshot“对象。

```language
// Recursive Function "Fifth rule" example

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       ...
    }
    // (Second rule)
    return retVal;
}
```

### 6. 递归函数分段
将当前阶段一分为二(针对当前只有单一递归调用的情形)。第一个阶段代表了下一次递归调用之前的情况，第二阶段代表了下一次递归调用完成并返回之后的情况(返回值已经被保存，并在此之前被累加)。
如果当前阶段有两次递归调用，就必须分为3个阶段。阶段1：第一次调用返回之前，阶段2：阶段1执行的调用过程。阶段3：第二次调用返回之前。
如果当前阶段有三次递归调用，就必须至少分为4个阶段。
依次类推。

```language
// Recursive Function "Sixth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          ...      // before ( SomeFunc(n-1, retIdx); )
          break; 
       case 1: 
          ...      // after ( SomeFunc(n-1, retIdx); )
          break;
       }
    }
    // (Second rule)
    return retVal;
}
```

### 7. 根据阶段变量stage的值切换到相应的处理流程并处理相关过程。

```language
// Recursive Function "Seventh rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    // (Third rule)
    stack<SnapShotStruct> snapshotStack;

    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage

    snapshotStack.push(currentSnapshot);

    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();

       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             ...
          }
          ...
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          break;
       }
    }
    // (Second rule)
    return retVal;
}
```

### 8. 保存返回时
如果递归有返回值，将这个值保存下来放在临时变量里面，比如retVal。当循环结束时，这个临时变量的值就是整个递归处理的结果。

```language
// Recursive Function "Eighth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             ...
          }
          ...
          // (Eighth rule)
          retVal = 0 ;
          ...
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          // (Eighth rule)
          retVal = currentSnapshot.test;
          ...
          break;
       }
    }
    // (Second rule)
    return retVal;
}
```

### 9. while-continue代替递归-return
如果递归函数有“return”关键字，你应该在while循环里面用“continue”代替。如果return了一个返回值，你应该在循环里面保存下来(步骤8)，然后return。大部分情况下，步骤九是可选的，但是它能帮助你避免逻辑错误。

```language
// Recursive Function "Ninth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             ...
          }
          ...
          // (Eighth rule)
          retVal = 0 ;
          
          // (Ninth rule)
          continue;
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          // (Eighth rule)
          retVal = currentSnapshot.test;

          // (Ninth rule)
          continue;
          break;
       }
    }
    // (Second rule)
    return retVal;
}
```

### 10. 递归结束生成新调用帧
为了模拟下一次递归函数的调用，你必须在当前循环函数里面再生成一个新的“Snapshot”结构体作为下一次调用的快照，初始化其参数以后压入栈，并“continue”。如果当前调用在执行完成后还有一些事情需要处理，那么更改它的阶段状态“stage”到相应的过程，并在new Snapshot压入之前，把本次的“Snapshot”压入。

```language
// Recursive Function "Tenth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             // (Tenth rule)
             currentSnapshot.stage = 1;            // - current snapshot need to process after
                                                   //     returning from the recursive call
             snapshotStack.push(currentSnapshot);  // - this MUST pushed into stack before 
                                                   //     new snapshot!
             // Create a new snapshot for calling itself
             SnapShotStruct newSnapshot;
             newSnapshot.n= currentSnapshot.n-1;   // - give parameter as parameter given
                                                   //     when calling itself
                                                   //     ( SomeFunc(n-1, retIdx) )
             newSnapshot.test=0;                   // - set the value as initial value
             newSnapshot.stage=0;                  // - since it will start from the 
                                                   //     beginning of the function, 
                                                   //     give the initial stage
             snapshotStack.push(newSnapshot);
             continue;
          }
          ...
          // (Eighth rule)
          retVal = 0 ;
          
          // (Ninth rule)
          continue;
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          // (Eighth rule)
          retVal = currentSnapshot.test;
          // (Ninth rule)
          continue;
          break;
       }
    }
    // (Second rule)
    return retVal;
}
```

## 5. 替代过程的几个简单例子
以下几个例子均在vs2008环境下开发，主要包含了：

### 1. 线性递归
```language
#ifndef __LINEAR_RECURSION_H__
#define __LINEAR_RECURSION_H__

#include <stack>
using namespace std;

/**
* \brief 求n的阶乘
* \para 
* \return 
* \note result = n! 递归实现
*/
int Fact(long n)
{
    if(0>n)
        return -1;
    if(0 == n)
        return 1;
    else
    {
        return ( n* Fact(n-1));
    }
} 

/**
* \brief 求n的阶乘
* \para 
* \return 
* \note result = n! 循环实现
*/
int FactLoop(long n)
{
    // (步骤1)
    struct SnapShotStruct // 快照结构体局部声明 
    {
        long inputN;      // 会改变的参数
                          // 没有局部变量
        int stage;        // 阶段变量用于快照跟踪
    } ;

    // (步骤2)
    int returnVal;        // 用于保存当前调用返回值  

    // (步骤3)
    stack<SnapShotStruct> snapshotStack;

    // (步骤4)
    SnapShotStruct currentSnapshot;
    currentSnapshot.inputN=n;
    currentSnapshot.stage=0; // 阶段变量初始化

    snapshotStack.push(currentSnapshot);  

    // (步骤5)
    while(!snapshotStack.empty())  
    {     
        currentSnapshot=snapshotStack.top();         
        snapshotStack.pop();       

        // (步骤6)
        switch(currentSnapshot.stage)
        {
            // (步骤7)
        case 0:
            if(0>currentSnapshot.inputN)
            {
                // (步骤8 & 步骤9)
                returnVal = -1;
                continue;
            }
            if(0 == currentSnapshot.inputN)
            {
                // (步骤8 & 步骤9)
                returnVal = 1;     
                continue;
            }
            else
            {
                // (步骤10)

                // 返回 ( n* Fact(n-1)); 分为2步： 
                // (第一步调用自身，第二步用返回值乘以当前n值)
                // 这里我们拍下快照.
                currentSnapshot.stage=1; // 当前的快照表示正在被处理，并等待自身调用结果返回，所以赋值为1 

                snapshotStack.push(currentSnapshot);

                // 创建一个新的快照，用于调用自身
                SnapShotStruct newSnapshot;
                newSnapshot.inputN= currentSnapshot.inputN -1 ; // 初始化参数 
                                                                 
                newSnapshot.stage = 0 ;                         // 从头开始执行自身，所以赋值stage==0 
                                                                
                snapshotStack.push(newSnapshot);
                continue;

            }
            break;
            // (步骤7)
        case 1:

            // (步骤8)

            returnVal = currentSnapshot.inputN * returnVal;

            // (步骤9)
            continue;
            break;
        }
    }
    
    // (步骤2)
    return returnVal;
}   
#endif //__LINEAR_RECURSION_H__
```

### 2. 二分递归
```language
#ifndef __BINARY_RECURSION_H__
#define __BINARY_RECURSION_H__

#include <stack>
using namespace std;

/**
* \function FibNum
* \brief 求斐波纳契数列
* \para 
* \return 
* \note 递归实现
*/
int FibNum(int n)
{
    if (n < 1)
        return -1;
    if (1 == n || 2 == n)
        return 1;

    // 这里可以看成是
    //int addVal = FibNum( n - 1);
    // addVal += FibNum(n - 2);
    // return addVal;
    return FibNum(n - 1) + FibNum(n - 2);                             
} 
/**
* \function FibNumLoop
* \brief 求斐波纳契数列
* \para 
* \return 
* \note 循环实现
*/
int FibNumLoop(int n)
{
    // (步骤1)
    struct SnapShotStruct // 快照结构体局部声明 
    {
        int inputN;       // 会改变的参数
        int addVal;       // 局部变量
        int stage;        // 阶段变量用于快照跟踪

    };

    // (步骤2)
    int returnVal;        // 用于保存当前调用返回值

    // (步骤3)
    stack<SnapShotStruct> snapshotStack;

    // (步骤4)
    SnapShotStruct currentSnapshot;
    currentSnapshot.inputN=n;
    currentSnapshot.stage=0; // 阶段变量初始化

    snapshotStack.push(currentSnapshot);

    // (步骤5)
    while(!snapshotStack.empty())
    {
        currentSnapshot=snapshotStack.top();
        snapshotStack.pop();

        // (步骤6)
        switch(currentSnapshot.stage)
        {
            // (步骤7)
        case 0:
            if(currentSnapshot.inputN<1)
            {
                // (步骤8 & 步骤9)
                returnVal = -1;
                continue;
            }
            if(currentSnapshot.inputN == 1 || currentSnapshot.inputN == 2 )
            {
                // (步骤8 & 步骤9)
                returnVal = 1;
                continue;
            }
            else
            {
                // (步骤10)

                // 返回 ( FibNum(n - 1) + FibNum(n - 2)); 相当于两步
                // (第一次调用参数是 n-1, 第二次调用参数 n-2)
                // 这里我们拍下快照，分成2个阶段
                currentSnapshot.stage=1;                        // 当前的快照表示正在被处理，并等待自身调用结果返回，所以赋值为1 

                snapshotStack.push(currentSnapshot);

                // 创建一个新的快照，用于调用自身
                SnapShotStruct newSnapshot;
                newSnapshot.inputN= currentSnapshot.inputN -1 ; //初始化参数 FibNum(n - 1)

                newSnapshot.stage = 0 ;                         
                snapshotStack.push(newSnapshot);
                continue;

            }
            break;
            // (步骤7)
        case 1:

            // (步骤10)

            currentSnapshot.addVal = returnVal;
            currentSnapshot.stage=2;                         // 当前的快照正在被处理，并等待的自身调用结果，所以阶段变量变成2

            snapshotStack.push(currentSnapshot);

            // 创建一个新的快照，用于调用自身
            SnapShotStruct newSnapshot;
            newSnapshot.inputN= currentSnapshot.inputN - 2 ; // 初始化参数 FibNum(n - 2)
            newSnapshot.stage = 0 ;                          // 从头开始执行，阶段变量赋值为0 
                                                             
            snapshotStack.push(newSnapshot);
            continue;
            break;
        case 2:
            // (步骤8)
            returnVal = currentSnapshot.addVal + returnVal;  // actual addition of ( FibNum(n - 1) + FibNum(n - 2) )

            // (步骤9)
            continue;
            break;
        }
    }  

    // (步骤2)
    return returnVal;
}
  

#endif //__BINARY_RECURSION_H__
```

### 3. 尾递归
```language
#ifndef __TAIL_RECURSION_H__
#define __TAIL_RECURSION_H__

#include <stack>
using namespace std;

/**
* \function FibNum2
* \brief 2阶裴波那契序列
* \para
* \return
* \note 递归实现  f0 = x, f1 = y, fn=fn-1+fn-2,   n=k,k+1,...
*/ 
int FibNum2(int n, int x, int y)
{   
    if (1 == n)    
    {
        return y;
    }
    else    
    {
        return FibNum2(n-1, y, x+y);
    }
}
/**
* \function FibNum2Loop
* \brief 2阶裴波那契序列
* \para
* \return
* \note 循环实现 在尾递归中, 递归调用后除了返回没有任何其它的操作, 所以在变为循环时，不需要stage变量
*/ 
int FibNum2Loop(int n, int x, int y)
{
    // (步骤1)
    struct SnapShotStruct 
    {
        int inputN;    // 会改变的参数
        int inputX;    // 会改变的参数
        int inputY;    // 会改变的参数
                       // 没有局部变量
    };

    // (步骤2)
    int returnVal;

    // (步骤3)
    stack<SnapShotStruct> snapshotStack;

    // (步骤4)
    SnapShotStruct currentSnapshot;
    currentSnapshot.inputN = n;
    currentSnapshot.inputX = x;
    currentSnapshot.inputY = y;

    snapshotStack.push(currentSnapshot);

    // (步骤5)
    while(!snapshotStack.empty())
    {
        currentSnapshot=snapshotStack.top();
        snapshotStack.pop();

        if(currentSnapshot.inputN == 1)
        {
            // (步骤8 & 步骤9)
            returnVal = currentSnapshot.inputY;
            continue;
        }
        else
        {
            // (步骤10)

            // 创建新快照
            SnapShotStruct newSnapshot;
            newSnapshot.inputN= currentSnapshot.inputN -1 ; // 初始化，调用( FibNum(n-1, y, x+y) )
            newSnapshot.inputX= currentSnapshot.inputY;
            newSnapshot.inputY= currentSnapshot.inputX + currentSnapshot.inputY;
            snapshotStack.push(newSnapshot);
            continue;
        }
    }
    // (步骤2)
    return returnVal;
} 

#endif //__TAIL_RECURSION_H__
```

### 4. 互递归
```language
#ifndef __MUTUAL_RECURSION_H__
#define __MUTUAL_RECURSION_H__
#include <stack>
using namespace std;

bool IsEvenNumber(int n);//判断是否是偶数
bool IsOddNumber(int n);//判断是否是奇数
bool isOddOrEven(int n, int stage);//判断是否是奇数或偶数

/****************************************************/
//互相调用的递归实现
bool IsOddNumber(int n)
{
    // 终止条件
    if (0 == n)
        return false;
    else
        // 互相调用函数的递归调用
        return IsEvenNumber(n - 1);
}

bool IsEvenNumber(int n)
{
    // 终止条件
    if (0 == n)
        return true;
    else
        // 互相调用函数的递归调用
        return IsOddNumber(n - 1);
} 


/*************************************************/
//互相调用的循环实现
bool IsOddNumberLoop(int n)
{
    return isOddOrEven(n , 0);
}

bool IsEvenNumberLoop(int n)
{
    return isOddOrEven(n , 1);
}

bool isOddOrEven(int n, int stage)
{
    // (步骤1)
    struct SnapShotStruct
    {
        int inputN;       // 会改变的参数
        int stage;
                          // 没有局部变量
    };

    // (步骤2)
    bool returnVal;       

    // (步骤3)
    stack<SnapShotStruct> snapshotStack;

    // (步骤4)
    SnapShotStruct currentSnapshot;
    currentSnapshot.inputN = n;
    currentSnapshot.stage = stage;

    snapshotStack.push(currentSnapshot);

    // (步骤5)
    while(!snapshotStack.empty())
    {
        currentSnapshot=snapshotStack.top();
        snapshotStack.pop();

        // (步骤6)
        switch(currentSnapshot.stage)
        {
            // (步骤7)
            // bool IsOddNumber(int n)
        case 0:
            // 终止条件
            if (0 == currentSnapshot.inputN)
            {
                // (步骤8 & 步骤9)
                returnVal = false;
                continue;
            }
            else
            {
                // (步骤10)

                // 模拟互调用的递归调用

                // 创建新的快照
                SnapShotStruct newSnapshot;
                newSnapshot.inputN= currentSnapshot.inputN - 1; // 初始化参数 
                // 调用 ( IsEvenNumber(n - 1) )
                newSnapshot.stage= 1;
                snapshotStack.push(newSnapshot);
                continue;
            }

            break;
            // (步骤7)
            // bool IsEvenNumber(int n)
        case 1:
            // 终止条件
            if (0 == currentSnapshot.inputN)
            {
                // (步骤8 & 步骤9)
                returnVal = true;
                continue;
            }
            else
            {
                // (步骤10)

                // 模拟互调用的递归调用

                // 创建新的快照
                SnapShotStruct newSnapshot;
                newSnapshot.inputN= currentSnapshot.inputN - 1; // 
                // calling itself ( IsEvenNumber(n - 1) )
                newSnapshot.stage= 0;
                snapshotStack.push(newSnapshot);
                continue;
            }
            break;
        }

    }
    // (步骤2)
    return returnVal;
}  

#endif //__MUTUAL_RECURSION_H__
```

### 5. 嵌套递归
```language
#ifndef __NESTED_RECURSION_H__
#define __NESTED_RECURSION_H__
#include <stack>
using namespace std;

int Ackermann(int x, int y)
{
    // 终止条件
    if (0 == x)
    {
        return y + 1;
    }   
    // 错误处理条件
    if (x < 0  ||  y < 0)
    {
        return -1;
    }  
    // 线性方法的递归调用 
    else if (x > 0 && 0 == y) 
    {
        return Ackermann(x-1, 1);
    }
    // 嵌套方法的递归调用
    else
    {
        //可以看成是：
        // int midVal = Ackermann(x, y-1);
        // return Ackermann(x-1, midVal);
        return Ackermann(x-1, Ackermann(x, y-1));
    }
}



int AckermannLoop(int x, int y)
{
    // (步骤1)
    struct SnapShotStruct 
    {
        int inputX;       // 会改变的参数
        int inputY;       // 会改变的参数
        int stage;
                          // 没有局部变量
    };

    // (步骤2)
    int returnVal;        

    // (步骤3)
    stack<SnapShotStruct> snapshotStack;

    // (步骤4)
    SnapShotStruct currentSnapshot;
    currentSnapshot.inputX = x;
    currentSnapshot.inputY = y;
    currentSnapshot.stage = 0;

    snapshotStack.push(currentSnapshot);

    // (步骤5)
    while(!snapshotStack.empty())
    {
        currentSnapshot=snapshotStack.top();
        snapshotStack.pop();

        // (步骤6)
        switch(currentSnapshot.stage)
        {
            // (步骤7)
        case 0:
            // 终止条件
            if(currentSnapshot.inputX == 0)
            {
                // (步骤8 & 步骤9)
                returnVal = currentSnapshot.inputY + 1;
                continue;             // 这里必须返回
            }
            // 错误处理条件        
            if (currentSnapshot.inputX < 0  ||  currentSnapshot.inputY < 0)
            {
                // (步骤8 & 步骤9)
                returnVal = -1;
                continue;             // 这里必须返回
            }  
            // 线性方法的递归调用 
            else if (currentSnapshot.inputX > 0 && 0 == currentSnapshot.inputY) 
            {
                // (步骤10)

                // 创建新快照
                SnapShotStruct newSnapshot;
                newSnapshot.inputX= currentSnapshot.inputX - 1; // 参数设定 calling itself ( Ackermann(x-1, 1) )
                newSnapshot.inputY= 1;                          // 参数设定 calling itself ( Ackermann(x-1, 1) )
                newSnapshot.stage= 0;
                snapshotStack.push(newSnapshot);
                continue;
            }
            // Recursive call by Nested method
            else
            {
                // (步骤10)

                currentSnapshot.stage=1;                       
                snapshotStack.push(currentSnapshot);

                // 创建新快照
                SnapShotStruct newSnapshot;
                newSnapshot.inputX= currentSnapshot.inputX;        //参数设定calling itself ( Ackermann(x, y-1) )
                newSnapshot.inputY= currentSnapshot.inputY - 1; //参数设定calling itself ( Ackermann(x, y-1) )
                newSnapshot.stage = 0;
                snapshotStack.push(newSnapshot);
                continue;
            }
            break;
        case 1:
            // (步骤10)

            // 创建新快照
            SnapShotStruct newSnapshot;
            newSnapshot.inputX= currentSnapshot.inputX - 1;   // 设定参数calling itself ( Ackermann(x-1,  Ackermann(x, y-1)) )
            newSnapshot.inputY= returnVal;                    // 设定参数calling itself ( Ackermann(x-1,  Ackermann(x, y-1)) )
            newSnapshot.stage = 0;
            snapshotStack.push(newSnapshot);
            continue;
            break;
        }
    }
    // (步骤2)
    return returnVal;
}     
#endif //__NESTED_RECURSION_H__
```

### 6. 测试代码
```language

```

## 6. 更多的例子
- [epQuickSort.h](https://github.com/juhgiyo/EpLibrary/blob/gh-pages/EpLibrary2.0/EpFoundation/Headers/epQuickSort.h)
- [epMergeSort.h](https://github.com/juhgiyo/EpLibrary/blob/gh-pages/EpLibrary2.0/EpFoundation/Headers/epMergeSort.h)
- [epKAryHeap.h](https://github.com/juhgiyo/EpLibrary/blob/gh-pages/EpLibrary2.0/EpFoundation/Headers/epKAryHeap.h)
- [epPatriciaTree.h](https://github.com/juhgiyo/EpLibrary/blob/gh-pages/EpLibrary2.0/EpFoundation/Headers/epPatriciaTree.h)


## 7. 结论
我的结论就是在c/c++或者Java代码中，尽量避免用递归。但是正如你看到的，递归容易理解，但是容易导致栈溢出。虽然循环版本的函数不会增加代码可读性和提升性能，但是它能有效的避免冲突或未定义行为。正如我开头所说，我的做法通常是在代码中写两份代码，一份递归，一份循环的。前者用于理解代码，后者用于实际的运行和测试用。如果你对于自己代码中使用这两种代码的利弊很清楚，你可以选择你自己的方式。

## 8. 参考

http://www.dreamincode.net/forums/topic/51296-types-of-recursion/
[EpLibrary 2.0](http://github.com/juhgiyo/EpLibrary)

## 9. 协议
本文及包含的代码遵从协议 [The MIT License](http://www.opensource.org/licenses/mit-license.php)

## 10. 其他
原文：http://www.codeproject.com/Articles/418776/How-to-replace-recursive-functions-using-stack-and
以上就是原文的一些内容，感谢原作者Woong Gyu La。
这篇文章中的代码我在调式过程中，发现了一个问题：循环版本的函数在执行效率方面存在问题。以后再改

# 原文


This article explains 10 rules (steps) for replacing the recursive functions using stack and while-loop to avoid the stack-overflow.
Download the recursive simulation samples - 12.4 KB
Table of contents
Introduction 
Purpose of Simulated function 
Pros and cons of Recursive and Simulated functions  
10 rules (steps) for replacing the recursive function using stack and while-loop   
First rule 
Second rule 
Third rule
Fourth rule
Fifth rule
Sixth rule
Seventh rule
Eighth rule
Ninth rule
Tenth rule 
Simple examples by types of recursion 
More practical example sources
Why do the sources contain both the simulated version and the recursive version?
Conclusion
Reference   
Introduction
There are cases where we prefer to use recursive functions such as sort (Merge Sort) or tree operations (heapify up / heapify down). However, if the recursive function goes too deep in some environments, such as in Visual C++ code, an unwanted result might occur such as a stack-overflow. Many professional developers probably already know how to replace recursive functions to avoid stack-overflow problems in advance by replacing with iterative function or using stack (heap stack) and while-loop (recursive simulation function). However I thought it would be a great idea to share simple and general methods (or guidelines) to replace the recursive functions using stack (heap stack) and while-loop to avoid the stack-overflow to help novice developers.    

Purpose of the Simulated function   
 

If you are using recursive function, since you don't have control on call stack and the stack is limited, the stack-overflow/heap-corruption might occur when the recursive call's depth gets too deep. The purpose of simulated function is moving the call stack to stack in heap, so the you can have more control on memory and process flow, and avoid the stack-overflow. It will be much better if it is replaced by iterative function, however in order to do that, it takes time and experience to handle every recursive function in proper way, so this article is a simple guide to help the novice developers to avoid the stack-overflow by using the recursive function, when they are not ready yet to handle everything in proper way.  

Pros and Cons of Recursive and Simulated function  
Recursive function 

Pros 
Very intuitive about the algorithm
See the examples in  RecursiveToLoopSamples.zip. 
Cons  
May occur "Stack-overflow," or "Heap corruption" 
Try to run IsEvenNumber function (Recursive) and IsEvenNumberLoop function (simulated) of "MutualRecursion.h" in  RecursiveToLoopSamples.zip with "10000" as its parameter input. 
Hide   Copy Code
#include "MutualRecursion.h"
 
bool result = IsEvenNumberLoop(10000); // returns successfully
bool result2 = IsEvenNumber(10000);     // stack-overflow error occurs 
Some people say that "(In order to fix the stack-overflow occurred by recursive function,) increase the MAX value of the stack to avoid the stack-overflow." However this is just temporary bandage, since if the recursive call gets deeper and deeper, the stack-overflow will most likely reappear. 

Simulated function 

Pros 
Can avoid "Stack-overflow," or "Heap corruption" errors.
More control on process flow and memory.  
Cons 
Not very intuitive about the algorithm.
Hard to maintain the code. 
10 Rules (steps) for replacing the recursive function with stack and while-loop  
First rule

Define a new struct called "Snapshot". The purpose of this data structure is to hold any data used in the recursive structure, along with any state information.
Things to include in the "Snapshot" structure.
the function argument that changes when the recursive function calls itself **However,** when the function argument is a reference, it does not need to be included in the Snapshot struct. Thus, in the following example, argument n should be included in the struct but argument retVal should not.
void SomeFunc(int n, int &retVal);
the "Stage" variable (usually an int to switch into the correct process divisions)
Read "Sixth rule" for detail.
local variables that will be used after returning from the function call (can happen during binary recursion or nested recursion)
Hide   Copy Code
// Recursive Function "First rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
} 
Hide   Copy Code
// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
    // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    ...
}
Second rule

Create a local variable at the top of the function. This value will represent the role of the return function in the recursive function.
in the iterative function, it is more like a temporary return value holder for each recursive call within the recursive function, since a C++ function can only have one return type.
if the recursive function's return type is void, then you can simply ignore creating this variable.
If there is any default return value then initialize this local variable with default value returning.
Hide   Copy Code
// Recursive Function "Second rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}
Hide   Copy Code
// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    ...
    // (Second rule)
    return retVal;
} 
Third rule

Make a stack with the "Snapshot" struct type.
Hide   Copy Code
// Recursive Function "Third rule" example

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    ...
    // (Second rule)
    return retVal;
}  
Fourth rule

Create a new "Snapshot" instance, and initialize with parameters that are input into the iterative function and the start "Stage" number.
Push the Snapshot instance into the empty stack.
Hide   Shrink    Copy Code
// Recursive Function "Fourth rule" example

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    // (Third rule)
    stack<SnapShotStruct> snapshotStack;

    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage

    snapshotStack.push(currentSnapshot);

    ...
    // (Second rule)
    return retVal;
}  
Fifth rule

Make a while loop which continues to loop while the stack is not empty.
At each iteration of the while loop, pop a Snapshot object from the stack
Hide   Shrink    Copy Code
// Recursive Function "Fifth rule" example

// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       ...
    }
    // (Second rule)
    return retVal;
}  
Sixth rule

Split the stages into two (for the case where there is only a single recursive call in the recursive function). The first case represents the code in the recursive function that is processed before the next recursive call is made, and the second case represents the code that is processed when the next recursive call returns (and when a return value is possibly collected or accumulated before the function returns it).
In the situation where there are two recursive calls within a function, there must be three stages:
** (Stage 1 --> recursive call --> (returned from first recursive call) Stage 2 (recursive call within stage 1)--> (return from second recursive call) Stage 3
In the situation where there are three different recursive calls then there must be at least 4 stages.
And so on.
Hide   Copy Code
// Recursive Function "Sixth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}
Hide   Shrink    Copy Code
// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          ...      // before ( SomeFunc(n-1, retIdx); )
          break; 
       case 1: 
          ...      // after ( SomeFunc(n-1, retIdx); )
          break;
       }
    }
    // (Second rule)
    return retVal;
} 
Seventh rule

Switch into the process division according to the " Stage " variable
Do the relevant process
Hide   Copy Code
// Recursive Function "Seventh rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}
Hide   Shrink    Copy Code
// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };

    // (Second rule)
    int retVal = 0;  // initialize with default returning value

    // (Third rule)
    stack<SnapShotStruct> snapshotStack;

    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage

    snapshotStack.push(currentSnapshot);

    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();

       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             ...
          }
          ...
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          break;
       }
    }
    // (Second rule)
    return retVal;
} 
Eighth rule

If there is a return type for the recursive function, store the result of the loop iteration in a local variable (such as retVal ).
This local variable will contain the final result of the recursive function when the while loop exits.
Hide   Copy Code
// Recursive Function "Eighth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}
Hide   Shrink    Copy Code
// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             ...
          }
          ...
          // (Eighth rule)
          retVal = 0 ;
          ...
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          // (Eighth rule)
          retVal = currentSnapshot.test;
          ...
          break;
       }
    }
    // (Second rule)
    return retVal;
} 
Ninth rule

In a case where there are "return" keywords within the recursive function, the  "return" keywords should be converted to "continue" within the "while" loop.
In a case where the recursive function returns a value, then as stated in "Eighth rule," store the return value in the local variable (such as retVal), and then "continue"
Most of the time, "Ninth rule" is optional, but it helps avoid logic error.
Hide   Copy Code
// Recursive Function "Ninth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}
Hide   Shrink    Copy Code
// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             ...
          }
          ...
          // (Eighth rule)
          retVal = 0 ;
          
          // (Ninth rule)
          continue;
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          // (Eighth rule)
          retVal = currentSnapshot.test;

          // (Ninth rule)
          continue;
          break;
       }
    }
    // (Second rule)
    return retVal;
} 
Tenth rule (and the last...)

To convert the recursive call within the recursive function, in iterative function, make a new "Snapshot" object, initialize the new "Snapshot" object stage, set its member variables according to recursive call parameters, and push into the stack, and "continue"
If there is process to be done after the recursive call, change the stage variable of current "Snapshot" object to the relevant stage, and  push into the stack before pushing the new "Snapshot" object into the stack.
Hide   Copy Code
// Recursive Function "Tenth rule" example
int SomeFunc(int n, int &retIdx)
{
   ...
   if(n>0)
   {
      int test = SomeFunc(n-1, retIdx);
      test--;
      ...
      return test;
   }
   ...
   return 0;
}
Hide   Shrink    Copy Code
// Conversion to Iterative Function
int SomeFuncLoop(int n, int &retIdx)
{
     // (First rule)
    struct SnapShotStruct {
       int n;        // - parameter input
       int test;     // - local variable that will be used 
                     //     after returning from the function call
                     // - retIdx can be ignored since it is a reference.
       int stage;    // - Since there is process needed to be done 
                     //     after recursive call. (Sixth rule)
    };
    // (Second rule)
    int retVal = 0;  // initialize with default returning value
    // (Third rule)
    stack<SnapShotStruct> snapshotStack;
    // (Fourth rule)
    SnapShotStruct currentSnapshot;
    currentSnapshot.n= n;          // set the value as parameter value
    currentSnapshot.test=0;        // set the value as default value
    currentSnapshot.stage=0;       // set the value as initial stage
    snapshotStack.push(currentSnapshot);
    // (Fifth rule)
    while(!snapshotStack.empty())
    {
       currentSnapshot=snapshotStack.top();
       snapshotStack.pop();
       // (Sixth rule)
       switch( currentSnapshot.stage)
       {
       case 0:
          // (Seventh rule)
          if( currentSnapshot.n>0 )
          {
             // (Tenth rule)
             currentSnapshot.stage = 1;            // - current snapshot need to process after
                                                   //     returning from the recursive call
             snapshotStack.push(currentSnapshot);  // - this MUST pushed into stack before 
                                                   //     new snapshot!
             // Create a new snapshot for calling itself
             SnapShotStruct newSnapshot;
             newSnapshot.n= currentSnapshot.n-1;   // - give parameter as parameter given
                                                   //     when calling itself
                                                   //     ( SomeFunc(n-1, retIdx) )
             newSnapshot.test=0;                   // - set the value as initial value
             newSnapshot.stage=0;                  // - since it will start from the 
                                                   //     beginning of the function, 
                                                   //     give the initial stage
             snapshotStack.push(newSnapshot);
             continue;
          }
          ...
          // (Eighth rule)
          retVal = 0 ;
          
          // (Ninth rule)
          continue;
          break; 
       case 1: 
          // (Seventh rule)
          currentSnapshot.test = retVal;
          currentSnapshot.test--;
          ...
          // (Eighth rule)
          retVal = currentSnapshot.test;
          // (Ninth rule)
          continue;
          break;
       }
    }
    // (Second rule)
    return retVal;
} 
Simple Examples by types of recursion  
Please download RecursiveToLoopSamples.zip
Unzip the file.
Open the project with Visual Studio.
This project has been developed with Visual Studio 2008
Sample project contains
Linear Recursion Example
Binary Recursion Example
Tail Recursion Example
Mutual Recursion Example
Nested Recursion Example
More Practical Example Sources  
The below sources contain both a recursive version and a simulated version, where the simulated version has been derived using the above methodology. 

epQuickSort.h
epMergeSort.h
epKAryHeap.h
epPatriciaTree.h
Why do the sources contain both the simulated version and the recursive version?  
If you look at the source, you can easily notice the simulated versions look much more complex than the recursive versions. For those who don't know what the function does, it will be much harder to figure out what the function with the loop actually does. So I prefer to keep both versions, so people can easily test out simple inputs and outputs with the recursive version, and for huge operations, use simulated version to avoid stack overflow. 

Conclusion   
My belief is that when writing C/C++ or Java code, the recursive functions MUST be used with care to avoid the stack-overflow error. However as you can see from the examples, in many cases, the recursive functions are easy to understand, and easy to write with the downside of "if the recursive function call's depth goes too deep, it leads to stack-overflow error". So conversion from recursive function to simulated function is not for increasing readability nor increasing algorithmic performance, but it is simple way of evading the crashes or undefined behaviors/errors. As I stated above, I prefer to keep both recursive version and simulated version in my code, so I can use the recursive version for readability and maintenance of the code, and the simulated version for running and testing the code.  It will be your choice how to write your code as long as you know the pros and cons for the choice, you are making.  

Reference   
http://www.dreamincode.net/forums/topic/51296-types-of-recursion/
EpLibrary 2.0 
History  
07.02.2015:- Broken link fixed
09.06.2013:- Typo fixed (Thanks to  lovewubo) 
08.22.2013:-  Re-distributed under MIT License from GPL v3 
08.10.2012: - Table of contents updated 
08.04.2012: - Moved the article's subsection to "Howto" 
07.23.2012: - Minor fixes on the article  
07.13.2012: - Table of contents modified 
Sections removed
Moved the article to Beginner section 
Changed the wording 
07.13.2012: - Table of contents added.
Titles modified.
New sections added.
Difference between Recursive and Iterative function
Pros and Cons of Recursive and Iterative approach
07.12.2012: - Sample bugs fixed.
Article re-organized.
Ninth and Tenth rule added.
Examples for each rule added.
07.11.2012: - Submitted the article.

License
This article, along with any associated source code and files, is licensed under The MIT License
