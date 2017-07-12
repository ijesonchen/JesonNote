---
title: stl chrono
date: 2017-05-24 22:14:00
tags: [stl, chrono]
---
STL Time library, The elements in this header deal with time. This is done mainly by means of three concepts: Durations, Time points and Clocks

<!--more-->

Durations 
They measure time spans, like: one minute, two hours, or ten milliseconds.
In this library, they are represented with objects of the duration class template, that couples a count representation and a period precision (e.g., ten milliseconds has ten as count representation and milliseconds as period precision).

Time points 
A reference to a specific point in time, like one's birthday, today's dawn, or when the next train passes.
In this library, objects of the time_point class template express this by using a duration relative to an epoch (which is a fixed point in time common to all time_point objects using the same clock). 

Clocks 
A framework that relates a time point to real physical time.
The library provides at least three clocks that provide means to express the current time as a time_point: system_clock, steady_clock and high_resolution_clock. 

see reference:
[chrono](http://www.cplusplus.com/reference/chrono/)

[TOC]


# duration and time_point

## duration

A duration object expresses a time span by means of a count and a period.

Internally, the object stores the count as an object of member type rep (an alias of the first template parameter, Rep), which can be retrieved by calling member function count.

This count is expresed in terms of periods. The length of a period is integrated in the type (on compile time) by its second template parameter (Period), which is a ratio type that expresses the number (or fraction) of seconds that elapse in each period.

Template instantiations
- hours
- minutes
- seconds
- milliseconds
- microseconds
- nanoseconds

## time_point
A time_point object expresses a point in time relative to a clock's epoch.

Internally, the object stores an object of a duration type, and uses the Clock type as a reference for its epoch.

member function:
time_since_epoch()

```language
  using namespace std::chrono;

  system_clock::time_point tp = system_clock::now();
  system_clock::duration dtn = tp.time_since_epoch();

  std::cout << "current time since epoch, expressed in:" << std::endl;
  std::cout << "periods: " << dtn.count() << std::endl;
  std::cout << "seconds: " << dtn.count() * system_clock::period::num / system_clock::period::den;
  std::cout << std::endl;
```

# clocks

## system_clock

member functions:


| function | note |
|--------|--------|
|now  | Get current time
|to_time_t  | tp to time_t
|from_time_t | time_t to tp



## steady_clock

## high_resolution_clock