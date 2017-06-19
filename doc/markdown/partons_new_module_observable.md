# New DVCS observable module {#newmodule_templates_dvcsobservable}

[TOC]

The implementation of this type of modules is slightly different for \f$\phi\f$-dependent observables (distingished by `ObservableType::PHI`) and for those \f$\phi\f$-integrated (disinguished by `ObservableType::FOURIER`). The good practice is to implement first a given \f$\phi\f$-dependent observable and then as many \f$\phi\f$-integrated derivatives as needed. This is ilustrated by the following templates.  

We are using the one-observable-is-one-module policy. It means that each module should correspond to only one physics observable. Therefore, such charecteristic of observable as beam charge, Fourier modulation type etc. can not be set from the outside (e.g. via `configure()` method), but it must be encoded in the module. The policy is crucial in particular for the database integrity, where the name of obserble allows to relate it to appropriate PARTONS module, so both of them must be unambiguous. 

# Header file template (phi-dependent observable) {#newmodule_templates_dvcsobservable_hdep}

For a detailed description of each virtual function we refer to its documentation, which is available after left-clicking on the function name. A short explanation is also available after hovering your mouse pointer on the name.  

~~~~~~~~~~~~~{.cpp}
class MyObservable: public Observable {

public:

    /*/** */* 
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId; 

    /*/** */* 
     * Default constructor.
     * @param className Name of class.
     */
    MyObservable(const std::string &className);

    /*/** */* 
     * Destructor.
     */
    virtual ~MyObservable();

    virtual MyObservable* clone() const;
    virtual double computePhiObservable(double phi);

protected:

    /*/** */* 
     * Copy constructor.
     * @param other Object to be copied.
     */
    MyObservable(const MyObservable &other);
};
~~~~~~~~~~~~~

# Source code file template (phi-dependent observable) {#newmodule_templates_dvcsobservable_cppdep}

~~~~~~~~~~~~~{.cpp}
const unsigned int MyObservable::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new MyObservable("MyObservable"));

MyObservable::MyObservable(const std::string &className) :
        Observable(className) {

    m_observableType = ObservableType::PHI;
    m_channel = ObservableChannel::DVCS;
}

MyObservable::MyObservable(const MyObservable& other) :
        Observable(other) {
}

MyObservable::~MyObservable() {
}

MyObservable* MyObservable::clone() const {
    return new MyObservable(*this);
}

double MyObservable::computePhiObservable(double phi) {

    //result
    double result;

    //your implementation comes here
    ...

    //return
    return result;
}

~~~~~~~~~~~~~

# Header file template (phi-integrated observable) {#newmodule_templates_dvcsobservable_hindep}

For a detailed description of each virtual function we refer to its documentation, which is available after left-clicking on the function name. A short explanation is also available after hovering your mouse pointer on the name.  

~~~~~~~~~~~~~{.cpp}
class MyObservableFourierModulation: public MyObservable, public MathIntegratorModule {

public:

    /*/** */* 
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId; 

    /*/** */* 
     * Default constructor.
     * @param className Name of class.
     */
    MyObservableFourierModulation(const std::string &className);

    /*/** */* 
     * Destructor.
     */
    virtual ~MyObservableFourierModulation();

    virtual MyObservableFourierModulation* clone() const;
    virtual double computeFourierObservable();

protected:

    /*/** */* 
     * Copy constructor.
     * @param other Object to be copied.
     */
    MyObservableFourierModulation(const MyObservableFourierModulation &other);

    /*/** */* 
     * Functor pointing to MyObservableFourierModulation::functionToIntegrateObservable().
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    /*/** */* 
     * Function to integrate over \f$\phi\f$ angle.
     */
    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    /*/** */* 
     * Initialize functors.
     */
    void initFunctorsForIntegrations();
};
~~~~~~~~~~~~~

# Source code file template (phi-integrated observable) {#newmodule_templates_dvcsobservable_cppindep}

~~~~~~~~~~~~~{.cpp}
const unsigned int MyObservableFourierModulation::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(new MyObservableFourierModulation("MyObservableFourierModulation"));

MyObservableFourierModulation::MyObservableFourierModulation(const std::string &className) :
        MyObservable(className), MathIntegratorModule() {

    m_observableType = ObservableType::FOURIER;
    m_channel = ObservableChannel::DVCS;

    setIntegrator(NumA::IntegratorType1D::DEXP);
    initFunctorsForIntegrations();
}

MyObservableFourierModulation::MyObservableFourierModulation(const MyObservableFourierModulation& other) :
        MyObservable(other), MathIntegratorModule(other) {
    initFunctorsForIntegrations();
}

MyObservableFourierModulation::~MyObservableFourierModulation() {

    if (m_pFunctionToIntegrateObservable) {
        delete m_pFunctionToIntegrateObservable;
        m_pFunctionToIntegrateObservable = 0;
    }
}

MyObservableFourierModulation* MyObservableFourierModulation::clone() const {
    return new MyObservableFourierModulation(*this);
}

void MyObservableFourierModulation::initFunctorsForIntegrations() {
    m_pFunctionToIntegrateObservable =
            NumA::Integrator1D::newIntegrationFunctor(this,
                    &MyObservableFourierModulation::functionToIntegrateObservable);
}

double MyObservableFourierModulation::functionToIntegrateObservable(double x,
        std::vector<double> params) {

    //your implementation comes here, e.g.:
    return MyObservable::computePhiObservable(x) * cos(x);
}

double MyObservableFourierModulation::computeFourierObservable() {

    //your implementation comes here, e.g.:
    std::vector<double> emptyParameters;

    return integrate(m_pFunctionToIntegrateObservable, 0., (2 * Constant::PI),
            emptyParameters) / Constant::PI;
}
~~~~~~~~~~~~~

# Useful variables {#newmodule_templates_dvcsobservable_var}

These are the most useful variables defined in the abstract classes. They are crucial for the implementation of new observable modules.

* Observable::m_pProcessModule: pointer to observable process being used 
