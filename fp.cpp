#include <iostream>
#include <memory>
#include <functional>
#include <map>

using namespace std;

class MyBaseClass {
public:
    virtual int doSomething() = 0;
};

class DerivedClassOne : public MyBaseClass {
public:
    DerivedClassOne() {};
    virtual ~DerivedClassOne() {};
    virtual int doSomething() { return 1; }
};

class DerivedClassTwo : public MyBaseClass {
public:
    DerivedClassTwo() {};
    virtual ~DerivedClassTwo() {};
    virtual int doSomething() { return 2; }
};


class MyFactory {
public:
    static MyFactory* Instance() {
        static MyFactory factory;
        return &factory;
    }

    shared_ptr<MyBaseClass> Create(string name) {
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

    void RegisterFactoryFunction(string name,
        std::function<MyBaseClass*(void)> classFactoryFunction) {
        // register the class factory function
        factoryFunctionRegistry[name] = classFactoryFunction;
    }
    map<string, function<MyBaseClass*(void)>> factoryFunctionRegistry;
};

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

int main()
{
    auto instanceOne = MyFactory::Instance()->Create("one");
    auto instanceTwo = MyFactory::Instance()->Create("two");

    MyFactory* myFactory = MyFactory::Instance();
    myFactory->Create("One")->doSomething();
    myFactory->Create("Two")->doSomething();

    return 0;
}
