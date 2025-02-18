# Task Scheduler

## Overview

A C++ task scheduler implementing computational graph execution with automatic dependency resolution.

## Features

  - **DAG Execution**: Automatically resolves task dependencies using topological sort
  - Custom `Any` type implementation with type erasure
  - Supports arbitrary callables (lambdas, functions, functors)
  - Works with copy-constructible arguments/return types

## Public Interface TTaskScheduler

 - **add** - accepts a task as an argument for it. Returns an object describing the added task.
 - **getFutureResult<T>** - returns an object from which in the future you can get the result of a task passed as a result of type T.
 - **getResult<T>** - returns the result of executing a task of a certain type. Calculate it if it hasn't already been calculated.
 - **executeAll** - executes all scheduled tasks.

## Execution Pipeline

1. **Task Registration**

User adds tasks with add() method, creating implicit graph edges through getFutureResult().

2. **Dependency Resolution**

Topological sort ensures tasks execute after their dependencies.

3. **Memoized Execution**

Results stored in type-erased containers for future access.

## Testing

All specified requirements are verified through comprehensive test coverage using the [Google Test](https://github.com/google/googletest) framework.

## Usage Examples

```cpp
TTaskScheduler scheduler;

float a = 1;
float b = -2;
float c = 0;

auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

scheduler.executeAll();

std::cout << "x1 = " << scheduler.getResult<float>(id5) << '\n'; // 2
std::cout << "x2 = " << scheduler.getResult<float>(id6) << '\n'; // 0
```

```cpp
TTaskScheduler scheduler;

float a = 10;
std::string s = "sqrt";

auto id1 = scheduler.add([](float a, std::string s){return a * a;}, a, s);
auto id2 = scheduler.add([](float a, float v){return a * v + a;}, a, scheduler.getFutureResult<float>(id1));

scheduler.executeAll();

std::cout << "Answer = " << scheduler.getResult<float>(id2) << '\n'; // 1010
```
