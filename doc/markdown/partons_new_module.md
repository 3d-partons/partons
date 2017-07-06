# New module {#newmodule}

[TOC]

# Introduction {#newmodule_intro}

PARTONS was conceived with the idea that anyone can plug his own models at any level of the computation chain. And this should be achieved without modifying the PARTONS library - one just need to write a class (in his own project) that inherits from an abstract class of a given type and implement virtual functions. This wiki page gives some hints on the creation of such new modules. In particular, it provides templates that illustrate structures to be understood and filled with the code by the developers.  

<hr>

# General remarks {#newmodule_remarks}

* Before any attempt of code writing, read [this wiki page](@ref usage) explaining the usage of PARTONS via C++ interface.

* PARTONS uses the registry mechanism. This imposes several requirements to be fulfilled by a module: 1) it must have `static const unsigned int classId` member initialized by `BaseObjectRegistry::registerBaseObject()` and a default constructor taking the module name, so it can be correctly registered in Registry, 2) it must have a copy constructor and the corresponding function `clone()` implemented, so it can be correctly issued by Registry, 3) if it calls other modules registered in Registry, they must be initialized in `resolveObjectDependencies()` and not in the constructor. 

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
With all set up, you can perform the integration:
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

* Module naming should be unambiguous and as straightforward as possible. However, it should be also informative, as the names will be used by the users to distinguish between many modules of a given type. For all modules except the observables we are using the following naming scheme: `ModuleType` + `UniqueName`, e.g. `DVCSProcessBMJ12`, `DVCSCFFHeavyQuark`, `GPDVinnikov06`. For observables we have: `ProcessType` + `ObservableName` + `BeamCharge` (+ `FourierModulation`), e.g. `DVCSAULMinus`, `DVCSAULMinusSin2Phi`.

<hr>

# Templates for new modules {#newmodule_templates}

* [new GPD module template](@ref newmodule_templates_gpd) 
* [new DVCS CFF module template](@ref newmodule_templates_dvcscff) 
* [new DVCS process module template](@ref newmodule_templates_dvcsprocess) 
* [new DVCS observable template](@ref newmodule_templates_dvcsobservable) 

<hr>

# How to use new module {#newmodule_usage}

After a proper building of your PARTONS-related project, you may use a new module as any other in PARTONS. See [this wiki page](@ref usage) for more information.
