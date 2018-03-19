#pragma once

#include "instance.h"
#include <cassert>

namespace global {



//replaces existing for the time it exised
template<typename T, typename Sub = typename Instance<T>::SubType>
class ReplacingInstanceRegistration {

public:

    ReplacingInstanceRegistration(){}
    ReplacingInstanceRegistration(T* t){registerInstance(t);}
    void operator()(T* t){registerInstance(t);}
    virtual ~ReplacingInstanceRegistration(){deregisterInstance();}

    virtual void registerInstance(T* t){
        deregisterInstance();
        instanceHasBeenReplaced = true;

        assert(replacedInstance==nullptr);

        if (isInstanceDefined<T,Sub>()){
            replacedInstance = &Instance<T,Sub>::get();
        } else {
            replacedInstance = nullptr;
        }

        Instance<T,Sub>::set(t); //possibly deregisters again
    }

    virtual void deregisterInstance(){
        if (instanceHasBeenReplaced==false) return; //noting to do
        instanceHasBeenReplaced = false;

        auto tmp = replacedInstance;
        replacedInstance = nullptr;

        Instance<T,Sub>::set(tmp); //possibly registers again
    }

private:

    ReplacingInstanceRegistration(ReplacingInstanceRegistration const&) = delete; //no copy

    T* replacedInstance = nullptr;
    bool instanceHasBeenReplaced = false;

};




class InstanceReplacementNotAllowed : public std::exception {};
class RegisteringNullNotAllowed: public std::exception {};



//expects nullptr to be registered beforehand
//expects registration-target not to be null
template<typename T, typename Sub = typename Instance<T>::SubType>
class InstanceRegistration : ReplacingInstanceRegistration<T,Sub> {
public:

    using Superclass = ReplacingInstanceRegistration<T,Sub>;
    using Superclass::operator();

    InstanceRegistration(): Superclass(){}
    InstanceRegistration(T* t): Superclass(){registerInstance(t);}

    void registerInstance(T* t) override{

        if (isInstanceDefined<T,Sub>()) throw InstanceReplacementNotAllowed();
        if (t==nullptr) throw RegisteringNullNotAllowed();

        Superclass::registerInstance(t);
    }


};

}//global
