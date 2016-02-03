#include "../../include/partons/ModuleObject.h"

//#include "../../include/partons/utils/stringUtils/Formatter.h"

ModuleObject::ModuleObject(const std::string &className) :
        BaseObject(className), Thread() {
}

ModuleObject::ModuleObject(const ModuleObject &other) :
        BaseObject(other), Thread(other) {
}

//TODO vérifier le cycle de vie des modules membres
ModuleObject::~ModuleObject() {
//    if (m_pMathIntegratorModule) {
//        delete m_pMathIntegratorModule;
//        m_pMathIntegratorModule = 0;
//    }
}

//TODO exception nothing to  configure
void ModuleObject::configure(ParameterList parameters) {

}

size_t ModuleObject::getNbOfParameters() {
    return m_parameters.size();
}

std::string ModuleObject::toString() const {
    return BaseObject::toString();
}

void ModuleObject::init() {

}
