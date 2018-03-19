#pragma once

#include <type_traits>
#include <src/InstanceRegistration.h>

namespace globalMocking{


constexpr bool testing = true;


struct A
{
    virtual int foo() { return system("rm /tmp/myfile"); }
};

struct A_mock  : public A
{
    int foo() override { return 0; }
};



struct B
{
    int bar(){ return global::instance<A>().foo() ? 77 : 66;}
};


void testB(){

    A_mock a_mock;

    global::ReplacingInstanceRegistration<A> reg(&a_mock); //temporarily make 'a_mock' globally accessible under 'A'

    B b;

    assert(b.bar() == 66);           //b.bar() uses instance of a_mock
}


void main_mockable(){

    A a;
    global::InstanceRegistration<A> reg(&a); //make 'a' globally accessible

    if (testing) testB();

    B b;
    b.bar(); //b.bar() uses instance of a
}


}//globalMocking