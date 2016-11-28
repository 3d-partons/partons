#include "../../include/partons/ModuleObject.h"

#include "../../include/partons/Partons.h"

const std::string ModuleObject::CLASS_NAME = "className";

ModuleObject::ModuleObject(const std::string &className) :
        BaseObject(className), ElemUtils::Thread(), m_pModuleObjectFactory(0), m_referenceModuleId(
                0) {
    m_pModuleObjectFactory = Partons::getInstance()->getModuleObjectFactory();
    m_referenceModuleId = getObjectId();
}

ModuleObject::ModuleObject(const ModuleObject &other) :
        BaseObject(other), ElemUtils::Thread(other) {
    m_pModuleObjectFactory = other.m_pModuleObjectFactory;
    m_referenceModuleId = other.m_referenceModuleId;
}

ModuleObject::~ModuleObject() {
    // Nothing to destroy
    // m_pModuleObjectFactory will be destroy by Partons class at close step.
}

void ModuleObject::configure(const ElemUtils::Parameters &parameters) {
    // Nothing to do
}

std::string ModuleObject::toString() const {
    return BaseObject::toString();
}

void ModuleObject::resolveObjectDependencies() {
    BaseObject::resolveObjectDependencies();
}

void ModuleObject::prepareSubModules(
        const std::map<std::string, BaseObjectData>& subModulesData) {
    // NOthing to do.
}

unsigned int ModuleObject::getReferenceModuleId() const {
    return m_referenceModuleId;
}

void ModuleObject::setReferenceModuleId(unsigned int referenceModuleId) {
    m_referenceModuleId = referenceModuleId;
}
