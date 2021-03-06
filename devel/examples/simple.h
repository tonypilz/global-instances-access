#pragma once

#include <src/globalInstances.h>
#include <iostream>

namespace simple {


struct A
{
    void foo(){std::cout<<"foo\n";}
};

void bar() {
    global::instance<A>()->foo();        // access global instance of A
}

void main_(){
    global::Instance<A> a; // make a globally accessible
    bar();
}



namespace withSingleton{


struct A
{
    static A& instance(){
        static A i;
        return i;
    }
    void foo(){std::cout<<"foo\n";}
};

void bar() {
    A::instance().foo();
}

void main_simple(){
    bar();
}

}
}
