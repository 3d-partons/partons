# New GPD module {#newmodule_templates_gpd}

[TOC]

# Header file template {#newmodule_templates_gpd_h}

For a detailed description of each virtual function we refer to its documentation, which is available after left-clicking on the function name. A short explanation is also provided by hovering your mouse pointer on the name.  

```cpp
class MyGPDModel: public PARTONS::GPDModule {

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

    virtual PARTONS::PartonDistribution computeH();
    virtual PARTONS::PartonDistribution computeE();
};
```

# Source code file template {#newmodule_templates_gpd_cpp}

```cpp
const unsigned int MyGPDModel::classId = 
    PARTONS::BaseObjectRegistry::getInstance()->registerBaseObject(new MyGPDModel("MyGPDModel"));

MyGPDModel::MyGPDModel(const std::string &className) : PARTONS::GPDModule(className) {

    //relate a specific GPD type with the appropriate function
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

    //result
    PARTONS::PartonDistribution result;

    //your implementation comes here
    ...

    //return
    return result;
}

PARTONS::PartonDistribution MyGPDModel::computeE() {
    //see compute::H()
}
```

# Useful variables {#newmodule_templates_gpd_var}

These are the most useful variables defined in the abstract classes. They are crucial for the implementation of new GPD modules.

* PARTONS::GPDModule::m_x, PARTONS::GPDModule::m_xi, PARTONS::GPDModule::m_t, PARTONS::GPDModule::m_MuF2, PARTONS::GPDModule::m_MuR2: indicate GPD kinematics being evaluated 
* PARTONS::GPDModule::m_gpdType: indicate GPD type being evaluated 
