#include "stdafx.h"
#include <iostream>
#include <memory>
#include <functional>
#include <fstream>
#include <string>
#include <vector>
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
    map<string, function<MyBaseClass*(void)>> factoryFunctionRegistry;
    void RegisterFactoryFunction(string name, function<MyBaseClass*(void)> factoryFunc) {
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
        else {
            return nullptr;
        }
    }    
};
int main() {
    MyFactory* myFactory = new MyFactory();
    myFactory->factoryFunctionRegistry["one"] = [](void)->MyBaseClass* {return new DerivedClassOne(); };
    myFactory->RegisterFactoryFunction("two", [](void)->MyBaseClass* {return new DerivedClassOne(); });

    Image images[5];
    myFactory->findMethod("one")->process("oneee", images[0]);

    ifstream myFile("cmds.txt");
    string line;
    vector<string> cmds;
    vector<string> params;
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            int breakPoint = line.find(" ");
            cmds.emplace_back(line.substr(0, breakPoint));
            params.emplace_back(line.substr(breakPoint+1));
        }
        myFile.close();
    }
    for (Image& image : images) {
        for (size_t i = 0; i < cmds.size(); i++) {
            myFactory->findMethod(cmds[i])->process(params[i], image);
        }
    }
    delete myFactory;
}
