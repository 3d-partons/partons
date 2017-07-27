#include "../../include/partons/ModuleObjectReference.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../include/partons/ModuleObject.h"

namespace PARTONS {


ModuleObjectReference::ModuleObjectReference(ModuleObject* pModuleObjectPointer) :
        BaseObject("ModuleObjectReference"), m_pModuleObjectPointer(
                pModuleObjectPointer), m_numberOfReference(1) {
}

ModuleObjectReference::ModuleObjectReference(const ModuleObjectReference& other) :
        BaseObject(other) {
    m_pModuleObjectPointer = other.m_pModuleObjectPointer;
    m_numberOfReference = other.m_numberOfReference;
}

ModuleObjectReference::~ModuleObjectReference() {
    if (m_pModuleObjectPointer != 0) {
        delete m_pModuleObjectPointer;
        m_pModuleObjectPointer = 0;
    }
}

void ModuleObjectReference::incCounter() {
    m_numberOfReference += 1;

    debug(__func__,
            ElemUtils::Formatter() << "Number of reference = "
                    << m_numberOfReference << " for "
                    << m_pModuleObjectPointer->getClassName());
}

void ModuleObjectReference::decCounter() {
    m_numberOfReference -= 1;

    debug(__func__,
            ElemUtils::Formatter() << "Number of reference = "
                    << m_numberOfReference << " for "
                    << m_pModuleObjectPointer->getClassName());
}

unsigned int ModuleObjectReference::getNumberOfReference() const {
    return m_numberOfReference;
}

void ModuleObjectReference::setNumberOfReference(
        unsigned int numberOfReference) {
    m_numberOfReference = numberOfReference;
}

const ModuleObject* ModuleObjectReference::getModuleObjectPointer() const {
    return m_pModuleObjectPointer;
}

std::string ModuleObjectReference::toString() const {
    return ElemUtils::Formatter() << "Referenced object("
            << m_pModuleObjectPointer->getClassName()
            << ") with number of reference(" << m_numberOfReference << ")";
}

} /* namespace PARTONS */
