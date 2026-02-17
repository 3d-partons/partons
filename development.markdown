# Development of a new module 

## Introduction 

PARTONS was designed with the philosophy that users should be able to "plug in" their own models at any level of the computation chain. This is achieved without modifying the core PARTONS library. A developer simply needs to create a class (direcly in PARTONS or within their own project) that inherits from an abstract class of a given type and implements the required virtual functions.

This tutorial provides guidance on creating these new modules and includes templates that illustrate the structures developers must understand and complete.

## General remarks

* **Prerequisites**: Before attempting to write any code, carefully read [this tutorial](usage.html) explaining the general usage of PARTONS.
* **External Development**: A child class representing your module can be written in an external program. You do not need to modify the PARTONS library directly, even though your class inherits from library members.
* **Registry/Factory Mechanism**: PARTONS uses a registry/factory pattern, which imposes the following requirements on new modules:
  * It must have a static const unsigned int classId member initialized by `BaseObjectRegistry::registerBaseObject()`.
  * It must have a default constructor that takes the module name to ensure correct registration in the Registry.
  * It must implement a copy constructor and a `clone()` function so the Factory can issue instances.
  * If the module calls other registered modules, they must be initialized in `resolveObjectDependencies()` rather than the constructor.
* **Virtual Functions**: You are required to implement all pure virtual functions (distinguished by `= 0` in the abstract class definition):
  ```cpp
  virtual void f() = 0;
  ```
  You do not need to implement other virtual functions unless you wish to override the default behavior defined in the parent class.
* **Execution Chain**: Be mindful of the execution chain when using inheritance. When necessary, follow the scheme below to ensure all parent class implementations are called:
  ```cpp
  C++
  #include <iostream>
  
  class A {
  public:
      virtual void f() {
          std::cout << "Base definition in A" << std::endl;
      }
  };
  
  class B : public A {
  public:
      virtual void f() {
          A::f(); // Call parent implementation
      }
  };
  
  class C : public B {
  public:
      virtual void f() {
          B::f(); // Call parent implementation
          std::cout << "Something new in C" << std::endl;
      }
  };
  
  int main() {
      A* pA = new C();
      pA->f();
      delete pA;
  }
  ```
  Output:
  ```Bash
  Base definition in A
  Something new in C
  ```
  While the implementation of `B::f()` isn't strictly necessary here (as it only calls `A::f()`), you should ensure `C::f()` calls its parent to avoid losing functionality, unless you specifically intend to override the entire chain.
* **Integration**: If your module requires numerical integration, utilize the MathIntegratorModule class. Inherit from this class and add as many functors as needed (functors represent the functions to be integrated):
  ```cpp
  class MyGPDModel: public GPDModule, public MathIntegratorModule { 
     // ...
     NumA::FunctionType1D* m_pFunctorForIntegrationFunction;
  };
  ```
  Initialize these functors in the constructor and set the default integration method in resolveObjectDependencies():
  ```cpp
  MyGPDModel::MyGPDModel(const std::string &className) 
      : GPDModule(className), MathIntegratorModule() {
     m_pFunctorForIntegrationFunction = NumA::Integrator1D::newIntegrationFunctor(this, &MyGPDModel::integrationFunction);
  }
  
  void MyGPDModel::resolveObjectDependencies() {
     setIntegrator(NumA::IntegratorType1D::DEXP);
  }
  ```
  The function to be integrated must follow this specific signature:
  ```cpp
  double MyGPDModel::integrationFunction(double x, std::vector<double> par) {
     // Implementation...
  }
  ```
  To perform the integration:
  ```cpp
  // Additional parameters passed to the function 
  std::vector<double> parameters; 
  double min = 0.;
  double max = 1.;
  
  // Do the integration
  double integrationResult = integrate(m_pFunctorForIntegrationFunction, min, max, parameters);
  ```
* **Naming Conventions**: Module names should be unambiguous, straightforward, and informative.
  * Standard Modules: Use ModuleType + UniqueName (e.g., `DVCSProcessBMJ12`, `GPDVinnikov06`).
  * Observables: Use ProcessType + ObservableName + BeamCharge + (FourierModulation), (e.g., `DVCSAULMinus`, `DVCSAULMinusSin2Phi`).
  * Note: If you plan to contribute your module to the official PARTONS library, these conventions are mandatory.
* **Documentation**: Always comment your code. If you are working directly on the PARTONS library, thorough documentation is an obligation.

## How to use a new module
Once your project is compiled, you can use your new module just like any other in PARTONS. For more details, see [this tutorial](usage.html). To instantiate a clone of your new GPD module, use the following:
```cpp
// Clone GPD module using the ModuleObjectFactory with your custom module's classId
PARTONS::GPDModule* pGPDModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyGPDModel::classId);
```

## Template for a new GPD module
### Header File Template
```cpp
class MyGPDModel : public PARTONS::GPDModule {

public:

    /** * Unique ID assigned by the BaseObjectRegistry.
     */
    static const unsigned int classId;

    /** * Default constructor.
     * @param className Name of the class.
     */
    MyGPDModel(const std::string& className);

    /** * Destructor.
     */
    virtual ~MyGPDModel();

    virtual MyGPDModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /** * Copy constructor.
     * @param other Object to be copied.
     */
    MyGPDModel(const MyGPDModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    // Specific GPD implementations
    virtual PARTONS::PartonDistribution computeH();
    virtual PARTONS::PartonDistribution computeE();
};
```

### Source Code File Template 
```cpp
// Register the module in the PARTONS Registry
const unsigned int MyGPDModel::classId = 
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new MyGPDModel("MyGPDModel"));

MyGPDModel::MyGPDModel(const std::string &className) : PARTONS::GPDModule(className) {

    // Map specific GPD types to their corresponding calculation functions
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(PARTONS::GPDType::H, &PARTONS::GPDModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(PARTONS::GPDType::E, &PARTONS::GPDModule::computeE));
}

MyGPDModel::MyGPDModel(const MyGPDModel& other) : PARTONS::GPDModule(other) {
}

MyGPDModel::~MyGPDModel() {
}

MyGPDModel* MyGPDModel::clone() const {
    return new MyGPDModel(*this);
}

void MyGPDModel::resolveObjectDependencies() {
    // Initialize sub-modules here if necessary
}

void MyGPDModel::configure(const ElemUtils::Parameters &parameters) {
    PARTONS::GPDModule::configure(parameters);
}

void MyGPDModel::isModuleWellConfigured() {
    PARTONS::GPDModule::isModuleWellConfigured();
}

void MyGPDModel::initModule() {
    PARTONS::GPDModule::initModule();
}

PARTONS::PartonDistribution MyGPDModel::computeH() {

    // Initialize result container
    PARTONS::PartonDistribution result;

    // TODO: Your model implementation comes here
    // Use kinematics variables (m_x, m_xi, etc.) to perform calculations

    return result;
}

PARTONS::PartonDistribution MyGPDModel::computeE() {
    // Implementation follows the same logic as computeH()
    PARTONS::PartonDistribution result;
    return result;
}
```
### Useful Variables
The following variables are defined in the parent abstract classes and are essential for implementing your GPD calculations. They store the current kinematic state for the evaluation:
* Kinematics:
  * `double m_x;` (x)
  * `double m_xi;`(xi)
  * `double m_t;` (t)
  * `double m_MuF2;` (factorization scale squared)
  * `double m_MuR2;` (renormalization scale squared)
* Type Context:
  * `GPDType::Type m_gpdType;` (indicates the specific GPD type currently being evaluated)
