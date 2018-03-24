#include "RegistrationTest.h"
#include <src/instance.h>
#include <src/InstanceRegistration.h>

RegistrationTest::RegistrationTest(QObject *parent) : QObject(parent)
{

}

void RegistrationTest::leavingTheScopeOfASingleInstanceRegistrationDeregistersInstance()
{
    class A{};
    A a;

    QVERIFY(global::instance<A>()==nullptr);
    {
        global::InstanceRegistration<A> registration(&a);
        QVERIFY(global::instance<A>()!=nullptr);
    }
    QVERIFY(global::instance<A>()==nullptr);
}

void RegistrationTest::leavingTheScopeOfASubSingleInstanceRegistrationDeregistersInstance()
{
    class A{};
    A a;

    class MySub{};

    {const bool def = global::instance<A,MySub>(); QCOMPARE(def,false);}
    {
        global::InstanceRegistration<A,MySub> registration(&a);
        {const bool def = global::instance<A,MySub>(); QCOMPARE(def,true);}
    }
    {const bool def = global::instance<A,MySub>(); QCOMPARE(def,false);}
}

void RegistrationTest::singleInstanceRegistrationAllowsOnlySingleRegistration()
{
    class A{};
    A a;

    global::InstanceRegistration<A> registration(&a);
    try {
        global::InstanceRegistration<A> registration(&a);
    }
    catch(global::InstanceReplacementNotAllowed const&){}
    catch(...){ QFAIL("");}

}

void RegistrationTest::singleInstanceSubRegistrationAllowsOnlySingleRegistration()
{
    class A{};
    A a;

    class MySub{};

    global::InstanceRegistration<A,MySub> registration(&a);
    try {
        global::InstanceRegistration<A,MySub> registration(&a);
    }
    catch(global::InstanceReplacementNotAllowed const&){}
    catch(...){ QFAIL("");}

}

void RegistrationTest::replacingInstanceRegistrationReplacesInstanceTemporarily()
{
    class A{};
    A a1,a2;

    global::InstanceRegistration<A> registration(&a1);
    QVERIFY(global::instance<A>()==&a1);
    {
        global::ReplacingInstanceRegistration<A> registration(&a2);
        QVERIFY(global::instance<A>()==&a2);
    }

   QVERIFY(global::instance<A>()==&a1);

}

void RegistrationTest::replacingInstanceSubRegistrationReplacesInstanceTemporarily()
{
    class A{};
    A a1,a2;

    class MySub{};

    global::InstanceRegistration<A,MySub> registration(&a1);

    {auto same = global::instance<A,MySub>()==&a1; QVERIFY(same );}
    {
        global::ReplacingInstanceRegistration<A,MySub> registration(&a2);
        {auto same = global::instance<A,MySub>()==&a2; QVERIFY(same );}
    }
    {auto same = global::instance<A,MySub>()==&a1; QVERIFY(same );}


}

void RegistrationTest::registrationsCanBeChanged()
{

    class A{};
    A a1,a2;

    global::ReplacingInstanceRegistration<A> registration(&a1);
    registration.registerInstance(&a2);

    {auto same = global::instance<A>()==&a2; QVERIFY(same);}

}

void RegistrationTest::registrationsSubCanBeChanged()
{
    class A{};
    A a1,a2;

    class MySub{};

    global::ReplacingInstanceRegistration<A,MySub> registration(&a1);
    registration.registerInstance(&a2);

    {auto same = global::instance<A,MySub>()==&a2; QVERIFY(same);}

}


