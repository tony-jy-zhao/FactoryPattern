// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <memory>
#include <functional>
#include <map>

using namespace std;

class Image{};
class MyBaseClass {
public:
    virtual void process(string param, Image& image) = 0;
};

class DerivedClassOne : public MyBaseClass {
public:
    DerivedClassOne() {};
    virtual ~DerivedClassOne() {};
    virtual void process(string param, Image& image) { cout << "One" << endl; }
};

class DerivedClassTwo : public MyBaseClass {
public:
    DerivedClassTwo() {};
    virtual ~DerivedClassTwo() {};
    virtual void process(string param, Image& image) { cout << "Two" << endl; }
};

class MyFactory {
public:
    shared_ptr<MyBaseClass> findMethod(string name) {
        MyBaseClass* instance = nullptr;

        // find name in the registry and call factory method.
        auto it = factoryFunctionRegistry.find(name);
        if (it != factoryFunctionRegistry.end())
        {
            instance = it->second();
        }

        // wrap instance in a shared ptr and return
        if (instance != nullptr)
        {
            return std::shared_ptr<MyBaseClass>(instance);
        }
        else
        {
            return nullptr;
        }
    }

    void RegisterFactoryFunction(string name, function<MyBaseClass*(void)> classFactoryFunction) {
        factoryFunctionRegistry[name] = classFactoryFunction;
    }
    map<string, function<MyBaseClass*(void)>> factoryFunctionRegistry;
};
/*
template<class T> class Registrar {
public:
    Registrar(string className) {
        // register the class factory function
        MyFactory::Instance()->RegisterFactoryFunction(className,
            [](void) -> MyBaseClass * { return new T(); });
    }
};

static Registrar<DerivedClassOne> registrar1("one");
static Registrar<DerivedClassTwo> registrar2("two");
*/
int main()
{
    MyFactory* myFactory = new MyFactory();
    myFactory->RegisterFactoryFunction("one", [](void) -> MyBaseClass * {return new DerivedClassOne(); });
    myFactory->RegisterFactoryFunction("two", [](void)-> MyBaseClass* {return new DerivedClassTwo(); });

    string p;
    Image img;
    myFactory->findMethod("one")->process(p,img);
    myFactory->findMethod("two")->process(p, img);

    return 0;
 }
