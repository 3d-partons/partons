# Adding a new model {#models}

PARTONS was conceived with the idea that anyone can plug his own models at any level of the computing chain.

### Example: Coefficient functions for DVCS

For example, if I want to compute a given Observable which will make use of a DVCSConvolCoeffFunctionModule, which itself calls a GPDModule, I can plug a custom DVCSCFF module without interfering with the rest of the chain.

This can be done without modifying the PARTONS library, we just need to write a class (in our own project) that will inherit from DVCSConvolCoeffFunctionModule. This child class needs to implement at least all the pure virtual methods of DVCSConvolCoeffFunctionModule.

### Header file

In that case, the basic header file of a custom module would probably look like this:

```cpp
#ifndef CUSTOM_DVCS_CFF_H
#define CUSTOM_DVCS_CFF_H

#include "path/to/DVCSConvolCoeffFunctionModule.h"
// Other necessary includes...

/**
 * Custom DVCS CFF (Compton Form Factor) module.
 */
class CustomDVCSCFF: public DVCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     @param className Name of class
     */
    CustomDVCSCFF(const std::string &className);

    /**
     * Clone
     */
    virtual CustomDVCSCFF* clone() const;

    /**
     * Destructor
     */
    virtual ~CustomDVCSCFF();

    virtual void configure(const ElemUtils::Parameters &parameters);
    
    virtual std::complex<double> computeCFF();

protected:

    /**
     * Copy constructor
     @param other Object to be copied
     */
    CustomDVCSCFF(const CustomDVCSCFF &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* CUSTOM_DVCS_CFF_H */
```

Some explanations:
* ffsdf
* sdsfsd
* sdsfsdf

### Source file

The source file would look like this:

```cpp
#include "path/to/CustomDVCSCFF.h"
// Other necessary includes...

// initialize [class]::classId with a unique name.
const unsigned int CustomDVCSCFF::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CustomDVCSCFF("CustomDVCSCFF"));

CustomDVCSCFF::CustomDVCSCFF(const std::string &className) :
        DVCSConvolCoeffFunctionModule(className) {
    //initialize
    m_listOfCFFComputeFunctionAvailable.insert(
                std::make_pair(GPDType::H,
                        &DVCSConvolCoeffFunctionModule::computeCFF));
}

// Copy constructor
CustomDVCSCFF::CustomDVCSCFF(const CustomDVCSCFF &other) :
        DVCSConvolCoeffFunctionModule(other) {
        
}

// Clone (used by the ModuleObjectFactory)
CustomDVCSCFF* CustomDVCSCFF::clone() const {
    return new CustomDVCSCFF(*this); // calls the copy constructor
}

//Destructor
CustomDVCSCFF::~CustomDVCSCFF() {
}

void CustomDVCSCFF::initModule() {

    // initialize parent module before
    DVCSConvolCoeffFunctionModule::initModule();
    
    // New initializations specific to the child class
}

void CustomDVCSCFF::isModuleWellConfigured() {

    // check parent module before
    DVCSConvolCoeffFunctionModule::isModuleWellConfigured();
    
    // New tests specific to the child class
}

std::complex<double> CustomDVCSCFF::computeCFF() {

}

void CustomDVCSCFF::configure(const ElemUtils::Parameters &parameters) {

}

```

Explanations:
* Blablabla

### How to use this new custom model

Now, it is simple, in the core of your program/script, you just need to call the ModuleObjectFactory and ask for your specific custom model:

```cpp
// Create CFF module
DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
        Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    CustomDVCSCFF::classId);
```

Note that we use pointers of the abstract class type, so that everything is generic. When this module will be given to a Service for example, it will know how to deal with it, despite the fact that we wrote a specific child class that PARTONS knows nothing about.