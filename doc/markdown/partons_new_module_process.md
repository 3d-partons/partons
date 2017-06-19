# New DVCS process module {#newmodule_templates_dvcsprocess}

[TOC]

# Header file template {#newmodule_templates_dvcsprocess_h}

For a detailed description of each virtual function we refer to its documentation, which is available after left-clicking on the function name. A short explanation is also available after hovering your mouse pointer on the name.  

~~~~~~~~~~~~~{.cpp}
class MyDVCSProcessModule: public DVCSModule {

public:

    /*/** */* 
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId; 

    /*/** */* 
     * Default constructor.
     * @param className Name of class.
     */
    MyDVCSProcessModule(const std::string &className);

    /*/** */* 
     * Destructor.
     */
    virtual ~MyCFFModel();
    virtual ~MyDVCSProcessModule();

    virtual MyDVCSProcessModule* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /*/** */* 
     * Copy constructor.
     * @param other Object to be copied.
     */
    MyDVCSProcessModule(const MyDVCSProcessModule& other);

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
~~~~~~~~~~~~~

# Source code file template {#newmodule_templates_dvcsprocess_cpp}

~~~~~~~~~~~~~{.cpp}
const unsigned int MyDVCSProcessModule::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new MyDVCSProcessModule("MyDVCSProcessModule"));

MyDVCSProcessModule::MyDVCSProcessModule(const std::string &className) :
        DVCSModule(className) {

    //this can be used to indicate that this DVCS process module does not need CFF module
    //default is that CFF module is needed, so the below line can be omitted
    isCCFModuleDependent(true);
}

MyDVCSProcessModule::MyDVCSProcessModule(const MyDVCSProcessModule& other) :
        DVCSModule(other) {
}

MyDVCSProcessModule::~MyDVCSProcessModule() {
}

MyDVCSProcessModule* MyDVCSProcessModule::clone() const {
    return new MyDVCSProcessModule(*this);
}

void MyDVCSProcessModule::resolveObjectDependencies() {
    DVCSModule::resolveObjectDependencies();
}
void MyDVCSProcessModule::configure(const ElemUtils::Parameters &parameters) {
    DVCSModule::configure(parameters);
}

void MyDVCSProcessModule::isModuleWellConfigured() {
    DVCSModule::isModuleWellConfigured();
}

void MyDVCSProcessModule::initModule() {
    DVCSModule::initModule();
}

void MyDVCSProcessModule::initModule(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    DVCSModule::initModule(beamHelicity, beamCharge, targetPolarization);
}

double MyDVCSProcessModule::CrossSectionBH(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {

    //result
    double result;

    //your implementation comes here
    ...

    //return
    return result; 
}

double MyDVCSProcessModule::CrossSectionVCS(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    //see MyDVCSProcessModule::CrossSectionBH()
}

double MyDVCSProcessModule::CrossSectionInterf(double beamHelicity, double beamCharge,
        NumA::Vector3D targetPolarization) {
    //see MyDVCSProcessModule::CrossSectionBH()
}
~~~~~~~~~~~~~

# Useful variables {#newmodule_templates_dvcsprocess_var} 

These are the most useful variables defined in the abstract classes. They are crucial for the implementation of new CFF modules.
* ProcessModule::m_xB, ProcessModule::m_t, ProcessModule::m_Q2, ProcessModule::m_E, ProcessModule::m_phi: indicate DVCS kinematics being currently evaluated
* DVCSConvolCoeffFunctionResult::m_dvcsConvolCoeffFunctionResult: DVCS CFFs calculated for kinematics being evaluated
