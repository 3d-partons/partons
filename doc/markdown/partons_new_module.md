# New module {#newmodule}

[TOC]

# Introduction {#newmodule_intro}

%PARTONS was conceived with the idea that anyone can plug his own models at any level of the computation chain. And this should be achieved without modifying the %PARTONS library - one just needs to write a class (in his own project) that inherits from an abstract class of a given type and implement virtual functions. This tutorial gives some hints on the creation of such new modules. In particular, it provides templates that illustrate structures to be understood and filled by the developers.  

<hr>

# General remarks {#newmodule_remarks}

To better understand the following remarks, please take a look in parallel at the examples provided in [the following section](@ref newmodule_templates).

* Before any attempt of code writing, read **carefully** [this tutorial](@ref usage) explaining the usage of %PARTONS.
* A child class representing your own module can be written in an external program, without a need of the modification of %PARTONS library, even though it inherits from classes being members of this library. The building of external programs is addressed in [the main usage tutorial](@ref usage).
* %PARTONS uses the registry / factory mechanism, which imposes several requirements to be met by a new module:
1. It must have `static const unsigned int classId` member initialized by `BaseObjectRegistry::registerBaseObject()` and a default constructor taking the module name, so it can be correctly registered in the Registry.
2. It must have a copy constructor and the corresponding function `clone()` implemented, so it can be correctly issued by the Factory.
3. If it calls other modules registered in the Registry, they must be initialized in `resolveObjectDependencies()` and not in the constructor.
* You are obliged to implement all pure virtual functions, which are distinguished by `= 0` in their abstract class definition, e.g.:
```cpp
virtual void f() = 0;
```
Formally, you do not need to implement other virtual functions, unless you want to change their default behavior defined in the parent class. 
* Be aware of the execution chain whenever the inheritance is involved. Whenever needed, keep the scheme illustrated by the following example, which allows to call all implementations of a given function that are defined in the parent classes: 
```cpp
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
```
which gives:
```sh
Base definition in A
Something new in C
```
The implementation of `B::f()` is not needed (it does nothing more than `A::f()`), but make sure to call it in the definition of `C:f()` as shown above to not lose any information, unless you precisely want to override `B::f()` and `A::f()`.
* Whenever an integration is needed, benefit from `MathIntegratorModule` class. Simply, inherit this class in your module and add as many functors as you like (functors represent integrated functions):
```cpp
class MyGPDModel: public GPDModule, public MathIntegratorModule { 
   ...
   NumA::FunctionType1D* m_pFunctorForIntegrationFunction;
}
```
Then, initialize these functors in the constructor and set the default integration method in `resolveObjectDependencies()` function:
```cpp
MyGPDModel::MyGPDModel(const std::string &className) : GPDModule(className), MathIntegratorModule() {
   ...
   m_pFunctorForIntegrationFunction = NumA::Integrator1D::newIntegrationFunctor(this, &MyGPDModel::integrationFunction);
}

void MyGPDModel::resolveObjectDependencies() {
   ...
   setIntegrator(NumA::IntegratorType1D::DEXP);
}
```
where `integrationFunction()` is the function to be integrated of the strict form:
```cpp
double MyGPDModel::integrationFunction(double x, std::vector<double> par) {
   ...
}
```
With all set up, you can perform the integration:
```cpp
//additional parameters passed to the function 
std::vector<double> parameters;

//integration range
double min = 0.;
double max = 1.;

//do the integration
double integrationResult = integrate(m_pFunctorForIntegrationFunction, min, max, parameters);
```
* Recommendation for your code: module naming should be unambiguous and as straightforward as possible. However, it should be also informative, as the names will be used by the users to distinguish between many modules of a given type. For all modules except the observables, we are using the following naming scheme: `ModuleType` + `UniqueName`, e.g. `DVCSProcessBMJ12`, `DVCSCFFHeavyQuark`, `GPDVinnikov06`. For observables we have: `ProcessType` + `ObservableName` + `BeamCharge` (+ `FourierModulation`), e.g. `DVCSAULMinus`, `DVCSAULMinusSin2Phi`. If you intend to extend %PARTONS with your new module and not just use it in your own project, this code recommendation becomes an obligation.
* Do not forget to comment your code! This becomes an obligation, if you are working directly on %PARTONS library. We are using Doxygen to provide documentation.

<hr>

# Templates for new modules {#newmodule_templates}

* [New GPD module](@ref newmodule_templates_gpd) 
* [New DVCS CFF module](@ref newmodule_templates_dvcscff) 
* [New DVCS process module](@ref newmodule_templates_dvcsprocess) 
* [New DVCS observable](@ref newmodule_templates_dvcsobservable) 

<hr>

# How to use new module {#newmodule_usage}

After a proper building of your %PARTONS-related project, you may use a new module as any other in %PARTONS - see [this tutorial](@ref usage) for more details. For instance, to get a clone of your new GPD module do as follows:
```cpp
// Clone GPD module with the ModuleObjectFactory from our own custom module
PARTONS::GPDModule* pGPDModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyGPDModel::classId);
```
