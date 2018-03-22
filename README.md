# Overview
This library improves the classical singleton pattern with respect to 
 - testing
 - arguments for the constructor 
 - control over seqcence and timepoint of construction/destruction
 - invalid access detection
 
It is achieved by decoupling instance access from the instance lifetime. The following example illustrates this:

```cpp
#include <src/InstanceRegistration.h>

void bar(); 

struct A
{
    void foo(){std::cout<<"foo\n";}
};

void main(){
    A a;                                     // 1) construct instance
    global::InstanceRegistration<A> reg(&a); // 2) make instance 'a' globally accessible
    bar();
}                                            // 4) 

void bar() {
    global::instance<A>().foo();             // 3) access instance  'a' 
}

```

In the example above, instance `a` is constructed as a regular object in `1`, then made globally accessible in `2` which is then accessed by function `bar()` in `3`. Note on line `4` that instance `a` is made globally inaccessible prior to its destruction. 

The example above showed the basic usage of this library. Not shown by the example were the aspects of [testing](#testing) and delayed access which will be discussed below.

# Status
## Tested on
 - gcc-7.2.0

## Compiles Under
 - icc-18
 - gcc-4.9.0 .. gcc-7.3
 - clang 3.4 .. clang 6.0
 
## Status
 - Implementation: complete (~200sloc)
 - Tests: complete
 - Documentation: incomplete


# Testing 
There are two basic cases to be considered:
 1. Testing a globally accessible class and 
 2. Testing code that acesses global instances

The first case is simple. As globally accessible classes are just regular classes they can be tested like a regular class.

The second case requires more attention. In order to test code that accesses global instances those instances usually need to be replaced by mocks during the test. How this can be achieved is illustrated in the following example:   

```cpp
struct A
{
    virtual int foo() { return system("rm /tmp/myfile"); }
};

struct A_mock  : public A
{
    int foo() override { return 0; }
};

int bar(){ 
    return global::instance<A>().foo() ? 77 : 66;              // 1) access to global instance
}  

void bar_test(){

    A_mock a_mock;                                             // 2)

    global::ReplacingInstanceRegistration<A> reg(&a_mock);     // 3) temporarily replace global instance by 'a-mock'

    assert(bar() == 66);                                       // 4) test bar b.bar() uses 'a_mock'
    
}                                                              // 5) undo of step 2) 

```
In the example above the function the function `bar()` is tested by the call in `4`. The function `bar()` accesses in `1` a global instance of type `A` which is therefore be replaced in `3` by the instance `a_mock` before executing the test. So the call to `bar()` in `4` leads to the call of `A_mock::foo()` which skips the syscall and returns `0`. 
