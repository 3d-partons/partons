# New GPD module {#newmodule_templates_gpd}

[TOC]

# Header file template {#newmodule_templates_gpd_h}

For a detailed description of each virtual function we refer to its documentation, which is available after left-clicking on the function name. A short explanation is also available after hovering your mouse pointer on the name.  

~~~~~~~~~~~~~{.cpp}
class MyGPDModel: public GPDModule {

public:

    /*/** */* 
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;

    /*/** */* 
     * Default constructor.
     * @param className Name of class.
     */
    MyGPDModel(const std::string& className);

    /*/** */* 
     * Destructor.
     */
    virtual ~MyGPDModel();

    virtual MyGPDModel* clone() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /*/** */* 
     * Copy constructor.
     * @param other Object to be copied.
     */
    MyGPDModel(const MyGPDModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
};
~~~~~~~~~~~~~

# Source code file template {#newmodule_templates_gpd_cpp}

~~~~~~~~~~~~~{.cpp}
const unsigned int MyGPDModel::classId = 
    BaseObjectRegistry::getInstance()->registerBaseObject(new MyGPDModel("MyGPDModel"));

MyGPDModel::MyGPDModel(const std::string &className) : GPDModule(className) {

    //relate a specific GPD type with the appropriate function
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));
}

MyGPDModel::MyGPDModel(const MyGPDModel& other) : GPDModule(other) {
}

MyGPDModel::~MyGPDModel() {
}

MyGPDModel* MyGPDModel::clone() const {
    return new MyGPDModel(*this);
}

void MyGPDModel::resolveObjectDependencies() {
}

void MyGPDModel::configure(const ElemUtils::Parameters &parameters) {
    GPDModule::configure(parameters);
}

void MyGPDModel::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

void MyGPDModel::initModule() {
    GPDModule::initModule();
}

PartonDistribution MyGPDModel::computeH() {

    //result
    PartonDistribution result;

    //your implementation comes here
    ...

    //return
    return result;
}

PartonDistribution MyGPDModel::computeE() {
    //see compute::H()
}
~~~~~~~~~~~~~

# Useful variables {#newmodule_templates_gpd_var}

These are the most useful variables defined in the abstract classes. They are crucial for the implementation of new GPD modules.

* GPDModule::m_x, GPDModule::m_xi, GPDModule::m_t, GPDModule::m_MuF2, GPDModule::m_MuR2: indicate GPD kinematics being currently evaluated 
* GPDModule::m_gpdType: indicate GPD type being evaluated 
