#include "InstanceTest.h"
#include <src/instance.h>
#include <src/instanceRegistration.h>

InstanceTest::InstanceTest(QObject *parent) : QObject(parent)
{

}

void InstanceTest::aRegisteredInstanceIsAccessible()
{
    class A{};
    A a;

    global::ExclusiveRegistration<A> registration;
    registration(&a);

    QCOMPARE(global::isInstanceDefined<A>(),true);
    QCOMPARE(&global::instance<A>(),&a);

}

void InstanceTest::anUnregisteredInstanceIsNotAccessible()
{
    class A{};
    QCOMPARE(global::isInstanceDefined<A>(),false);
}

void InstanceTest::anUnregisteredSubInstanceIsNotAccessible()
{
    class A{};
    A a;

    global::ExclusiveRegistration<A> registration;
    registration(&a);

    class Sub{};

    const bool def = global::isInstanceDefined<A,Sub>();
    QCOMPARE(def,false);

}

void InstanceTest::aRegisteredSubInstanceIsAccessible()
{
    class A{};
    A a;

    class Sub{};

    global::ExclusiveRegistration<A,Sub> registration;
    registration(&a);


    const bool def = global::isInstanceDefined<A,Sub>();
    QCOMPARE(def,true);
}

void InstanceTest::aDerivedInstanceIsAccessibleWithoutSlicing()
{
    constexpr int val = 77;
    struct A{virtual ~A() = default;};
    struct B : public A{ int x = val; };
    B b;

    global::ExclusiveRegistration<A> registration(&b);


    auto res = dynamic_cast<B&>(global::instance<A>());

    QCOMPARE(res.x,val);

}

void InstanceTest::aDerivedSubInstanceIsAccessibleWithoutSlicing()
{
    class Sub{};

    constexpr int val = 77;
    struct A{virtual ~A() = default;};
    struct B : public A{ int x = val; };
    B b;

    global::ExclusiveRegistration<A,Sub> registration(&b);


    auto res = dynamic_cast<B&>(global::instance<A,Sub>());

    QCOMPARE(res.x,val);
}

void InstanceTest::gettingNullThrowsWithoutHandler()
{
    class A{};

    try{
        global::instance<A>();
    }
    catch(global::NullptrAccess const&){}
    catch(...){ QFAIL("");}
}

void InstanceTest::gettingNullInvokesInstalledUntypeHandler()
{
    class A{};

    class UntypedTestHandler : public std::exception {};

    global::instanceHandler::nullptrAccessHandler() = [](){ throw UntypedTestHandler();};


    try{
        global::instance<A>();
    }
    catch(UntypedTestHandler const&){}
    catch(...){ QFAIL("");}

    global::instanceHandler::nullptrAccessHandler() = std::function<void()>{}; //cleanup installed handler
}

void InstanceTest::gettingNullInvokesInstalledTypeHandlerBeforeUntyped()
{
    class A{};
    A a;

    class UntypedTestHandler : public std::exception {};
    class TypedTestHandler : public std::exception {};

    auto& hu = global::instanceHandler::nullptrAccessHandler();
    auto& ht = global::instanceHandler::nullptrAccessHandler<A,global::Instance<A>::SubType>();


    hu = [](){ throw UntypedTestHandler();};
    ht = [&a]()->A*{ return &a;};

    QCOMPARE(&global::instance<A>(),&a);

    hu = std::function<void()>{}; //cleanup installed handler
    ht = std::function<A*()>{}; //cleanup installed handler

}


void InstanceTest::noDoubleNotifications()
{
    class A{};
    A a;

    global::ExclusiveRegistration<A> registration(&a);

    int calls = 0;
    auto& h = global::instanceHandler::instanceChangedHandler();
    h = [&calls](){ ++calls; };

    QCOMPARE(calls,0);
    { global::ReplacingScopedRegistration<A> registration(&a);}
    QCOMPARE(calls,0);

    h = std::function<void()>{}; //cleanup installed handler

}


void InstanceTest::instanceChangedHandlersTriggered()
{
    class A{};
    A a;

    int callsu = 0;
    int callst = 0;
    auto& hu = global::instanceHandler::instanceChangedHandler();
    auto& ht = global::instanceHandler::instanceChangedHandler<A,global::Instance<A>::SubType>();

    A* expect = &a;
    hu = [&callsu](){ ++callsu; };
    ht = [&callst,&expect](A* t){ ++callst; QCOMPARE(t,expect); };

    {

        global::ExclusiveRegistration<A> registration;

        registration(&a);

        QCOMPARE(callsu,1);
        QCOMPARE(callst,1);


        {
            A b;
            expect = &b;

            global::ReplacingScopedRegistration<A> registration;

            registration(&b);

            QCOMPARE(callsu,2);
            QCOMPARE(callst,2);

            expect = &a;

        }


        QCOMPARE(callsu,3);
        QCOMPARE(callst,3);

        expect = nullptr;
    }



    QCOMPARE(callsu,4);
    QCOMPARE(callst,4);

    hu = std::function<void()>{}; //cleanup installed handler
    ht = std::function<void(A*)>{}; //cleanup installed handler
}


