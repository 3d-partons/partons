# Adding a new model {#newmodule}

[TOC]

PARTONS was conceived with the idea that anyone can plug his own models at any level of the computation chain. And this should be achieved without modifying the PARTONS library - one just need to write a class (in his own project) that inherits from an abstract one of a given type and implement virtual functions. This wiki page gives some hints on the creation of such new modules. In particular, it provides templates that illustrate structures to be understood and filled with the code by the developers.  

# General remarks {#newmodule_remarks}

* PARTONS uses the registry mechanism. This imposes several requirements to be fulfilled by a module: 1) it must have `static const unsigned int classId` member initialized by `BaseObjectRegistry::registerBaseObject()` and a default constructor taking the module name, so it can be correctly registered in the registry, 2) it must have a copy constructor and the corresponding function `clone()` implemented, so it can be correctly issued by the registry, 3) if it calls other modules registered in the registry, they must be initialized in `resolveObjectDependencies()` and not in the constructor. 

* PARTONS uses the logger mechanism. Use it whenever needed, instead of functions like `printf()`, `std::cout`, etc. This will allow you to create a consistent output, but it is also crucial to keep the continuity of the information stream - since the logger uses a different thread than the computation, information send by the logger and by e.g. `printf()` function in your code will not be aligned correctly. 

* Whenever an integration is needed, benefit from `MathIntegratorModule` class. Simply, inherit this class in your module and add as many functors as you like (functors represent the integrated functions):
~~~~~~~~~~~~~{.cpp}
class MyGPDModel: public GPDModule, public MathIntegratorModule { 
   ...
   NumA::FunctionType1D* m_pFunctorForIntegrationFunction;
}
~~~~~~~~~~~~~
Then, initialize these functors and set the default integration method in the constructor:
~~~~~~~~~~~~~{.cpp}
MyGPDModel::MyGPDModel(const std::string &className) : GPDModule(className), MathIntegratorModule() {
   ...
   m_pFunctorForIntegrationFunction = NumA::Integrator1D::newIntegrationFunctor(this, &MyGPDModel::integrationFunction);
   setIntegrator(NumA::IntegratorType1D::DEXP);
}
~~~~~~~~~~~~~
where `integrationFunction()` is the function to be integrated of the strict form:
~~~~~~~~~~~~~{.cpp}
double MyGPDModel::integrationFunction(double x, std::vector<double> par) {
   ...
}
~~~~~~~~~~~~~
Finally, perform the integration:
~~~~~~~~~~~~~{.cpp}
//additional parameters passed to the function 
std::vector<double> parameters;

//integration range
double min = 0.;
double max = 1.;

//do the integration
double integrationResult = integrate(m_pFunctorForIntegrationFunction, min, max, parameters);
~~~~~~~~~~~~~

* Be aware of the execution chain whenever the inheritance is involved. Whenever needed, keep the scheme illustrated by the following example, which allows to call all implementations of a given function that are defined in the abstract classes: 
~~~~~~~~~~~~~{.cpp}
#include <iostram>

class A{
public:
	virtual void f(){
		std::cout << "Base definition in A" << std::end;
	}
};

class B : public A{
public:
	virtual void f(){
		A::f();		
	}
};

class C : public B{
public:
	virtual void f(){
		B::f();
		std::cout << "Something new in C" << std::endl;
	}
};

int main(){
	A* pA = new C();
	pA->f();
}
~~~~~~~~~~~~~
which gives:
~~~~~~~~~~~~~{.cpp}
Base definition in A
Something new in C
~~~~~~~~~~~~~

* Do not forget to comment your code! We are using Doxygen.

* Module naming should be unambiguous and as straightforward as possible. However, it should be also helpful for the users, as they will use these names to select the modules. Therefore, we are using the following naming scheme for all modules except the observables: `ModuleType` + `UniqueName`, e.g. `DVCSProcessBMJ12Model`, `DVCSCFFHeavyQuarkModel`, `GPDVinnikov06`. For observables we have: `ProcessType` + `ObservableName` + `BeamCharge` (+ `FourierModulation`), e.g. `DVCSAULMinus`, `DVCSAULMinusSin2Phi`.

# Templates for new modules {#newmodule_templates}

* [new GPD module template](@ref newmodule_templates_gpd) 
* [new DVCS CFF module template](@ref newmodule_templates_dvcscff) 
* [new DVCS process module template](@ref newmodule_templates_dvcsprocess) 
* [new DVCS observable template](@ref newmodule_templates_dvcsobservable) 

# How to use this new custom model {#newmodule_usage}

Now, it is simple, in the core of your program/script, you just need to call the ModuleObjectFactory and ask for your specific custom model:

```cpp
// Create CFF module
DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
        Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    CustomDVCSCFF::classId);
```

Note that we use pointers of the abstract class type, so that everything is generic. When this module will be given to a Service for example, it will know how to deal with it, despite the fact that we wrote a specific child class that PARTONS knows nothing about.
