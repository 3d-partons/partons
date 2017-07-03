# New DVCS CFF module {#newmodule_templates_dvcscff}

[TOC]

# Header file template {#newmodule_templates_dvcscff_h}

For a detailed description of each virtual function we refer to its documentation, which is available after left-clicking on the function name. A short explanation is also provided by hovering your mouse pointer on the name.  

~~~~~~~~~~~~~{.cpp}
class MyCFFModel: public DVCSConvolCoeffFunctionModule {

public:

    /*/** */* 
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId; 

    /*/** */* 
     * Default constructor.
     * @param className Name of class.
     */
    MyCFFModel(const std::string &className);

    /*/** */* 
     * Destructor.
     */
    virtual ~MyCFFModel();

    virtual MyCFFModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /*/** */* 
     * Copy constructor.
     * @param other Object to be copied.
     */
    MyCFFModel(const MyCFFModel &other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();
};
~~~~~~~~~~~~~

# Source code file template {#newmodule_templates_dvcscff_cpp}

~~~~~~~~~~~~~{.cpp}
const unsigned int MyCFFModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new MyCFFModel("MyCFFModel"));

MyCFFModel::MyCFFModel(const std::string& className) :
        DVCSConvolCoeffFunctionModule(className) {

    //relate a specific GPD type with the appropriate function
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DVCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DVCSConvolCoeffFunctionModule::computeUnpolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DVCSConvolCoeffFunctionModule::computePolarized));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DVCSConvolCoeffFunctionModule::computePolarized));

    //this can be used to indicate that this CFF module does not need GPD module
    //default is that GPD module is needed, so the below line can be omitted
    setIsGPDModuleDependent(true);
}

MyCFFModel::MyCFFModel(const MyCFFModel& other) : DVCSConvolCoeffFunctionModule(other) {
}

MyCFFModel::~MyCFFModel() {
}

MyCFFModel* MyCFFModel::clone() const {
    return new MyCFFModel(*this);
}

void MyCFFModel::resolveObjectDependencies() {
    DVCSConvolCoeffFunctionModule::resolveObjectDependencies();
}

void MyCFFModel::configure(const ElemUtils::Parameters &parameters) {
    DVCSConvolCoeffFunctionModule::configure(parameters);
}

void MyCFFModel::isModuleWellConfigured() {
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
}

void MyCFFModel::initModule() {
    DVCSConvolCoeffFunctionModule::initModule();
}

std::complex<double> MyCFFModel::computeUnpolarized() {

    //result
    std::complex<double> result;

    //your implementation comes here

    //return
    return result; 
}

std::complex<double> MyCFFModel::computePolarized() {
    //see MyCFFModel::computeUnpolarized()
}
~~~~~~~~~~~~~

# Useful variables {#newmodule_templates_dvcscff_var} 

These are the most useful variables defined in the abstract classes. They are crucial for the implementation of new CFF modules.

* DVCSConvolCoeffFunctionModule::m_xi, DVCSConvolCoeffFunctionModule::m_t, DVCSConvolCoeffFunctionModule::m_MuF2, DVCSConvolCoeffFunctionModule::m_MuR2: indicate CFF kinematics being evaluated 
* DVCSConvolCoeffFunctionModule::m_currentGPDComputeType: indicate GPD type being evaluated 
* DVCSConvolCoeffFunctionModule::m_qcdOrderType: indicate pQCD order being evaluated 
* ConvolCoeffFunctionModule::m_pGPDModule: pointer to GPD module being used 
