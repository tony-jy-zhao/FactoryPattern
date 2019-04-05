#include <iostream>
#include <memory>
#include <functional>
#include <map>

using namespace std;
class Image {};
class MyBaseClass {
public:
    virtual void process(string param, Image& image) = 0;
};

class DerivedClassOne : public MyBaseClass {
public:
    virtual void process(string param, Image& image) { cout << "One" << endl; }
};

class DerivedClassTwo : public MyBaseClass {
public:
    virtual void process(string param, Image& image) { cout << "Two" << endl; }
};

class MyFactory {
public:
    map<string, function<MyBaseClass*()>> factoryFunctionRegistry;
    void RegisterFactoryFunction(string name, function<MyBaseClass*()> factoryFunc) {
        factoryFunctionRegistry[name] = factoryFunc;
    }
    shared_ptr<MyBaseClass> findMethod(string name) {
        MyBaseClass* instance = nullptr;

        auto it = factoryFunctionRegistry.find(name);
        if (it != factoryFunctionRegistry.end()) {
            instance = it->second();
        }
        if (instance != nullptr) {
            return shared_ptr<MyBaseClass>(instance);
        }
    }
};
