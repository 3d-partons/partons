# New DVCS process module {#newmodule_templates_dvcsprocess}

[TOC]

# Header file template {#newmodule_templates_dvcsprocess_h}

For a detailed description of each virtual function we refer to its documentation, which is available after left-clicking on the function name. A short explanation is also provided by hovering your mouse pointer on the name.  

```cpp
class MyDVCSProcess: public PARTONS::DVCSModule {

public:

    /*/** */* 
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId; 

    /*/** */* 
     * Default constructor.
     * @param className Name of class.
     */
    MyDVCSProcess(const std::string &className);

    /*/** */* 
     * Destructor.
     */
    virtual ~MyCFFModel();
    virtual ~MyDVCSProcess();

    virtual MyDVCSProcess* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /*/** */* 
     * Copy constructor.
     * @param other Object to be copied.
     */
    MyDVCSProcess(const MyDVCSProcess& other);

    virtual void initModule();
    virtual void initModule(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual void isModuleWellConfigured();

    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
};
```

# Source code file template {#newmodule_templates_dvcsprocess_cpp}

```cpp
const unsigned int MyDVCSProcess::classId =
        PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new MyDVCSProcess("MyDVCSProcess"));

MyDVCSProcess::MyDVCSProcess(const std::string &className) :
        PARTONS::DVCSModule(className) {

    //this can be used to indicate that this DVCS process module does not need CFF module
    //default is that CFF module is needed, so the below line can be omitted
    isCCFModuleDependent(true);
}

MyDVCSProcess::MyDVCSProcess(const MyDVCSProcess& other) :
        PARTONS::DVCSModule(other) {
}

MyDVCSProcess::~MyDVCSProcess() {
}

MyDVCSProcess* MyDVCSProcess::clone() const {
    return new MyDVCSProcess(*this);
}

void MyDVCSProcess::resolveObjectDependencies() {
    PARTONS::DVCSModule::resolveObjectDependencies();
}
void MyDVCSProcess::configure(const ElemUtils::Parameters &parameters) {
    PARTONS::DVCSModule::configure(parameters);
}

void MyDVCSProcess::isModuleWellConfigured() {
    PARTONS::DVCSModule::isModuleWellConfigured();
}

void MyDVCSProcess::initModule() {
    PARTONS::DVCSModule::initModule();
}

void MyDVCSProcess::initModule(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    PARTONS::DVCSModule::initModule(beamHelicity, beamCharge, targetPolarization);
}

double MyDVCSProcess::CrossSectionBH(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    //result
    double result;

    //your implementation comes here
    ...

    //return
    return result; 
}

double MyDVCSProcess::CrossSectionVCS(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    //see MyDVCSProcess::CrossSectionBH()
}

double MyDVCSProcess::CrossSectionInterf(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    //see MyDVCSProcess::CrossSectionBH()
}
```

# Useful variables {#newmodule_templates_dvcsprocess_var} 

These are the most useful variables defined in the abstract classes. They are crucial for the implementation of new CFF modules.
* PARTONS::ProcessModule::m_xB, PARTONS::ProcessModule::m_t, PARTONS::ProcessModule::m_Q2, PARTONS::ProcessModule::m_E, PARTONS::ProcessModule::m_phi: indicate DVCS kinematics being evaluated
* PARTONS::DVCSConvolCoeffFunctionResult::m_dvcsConvolCoeffFunctionResult: DVCS CFFs calculated for kinematics being evaluated
