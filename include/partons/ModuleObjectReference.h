#ifndef MODULE_OBJECT_REFERENCE_H
#define MODULE_OBJECT_REFERENCE_H

/**
 * @file ModuleObjectReference.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 23, 2016
 * @version 1.0
 */

#include <string>

#include "BaseObject.h"

namespace PARTONS {

class ModuleObject;

/**
 * @class ModuleObjectReference
 */
class ModuleObjectReference: public BaseObject {
public:

    /**
     *
     * @param pModuleObjectPointer
     */
    ModuleObjectReference(ModuleObject* pModuleObjectPointer);

    /**
     * Copy constructor
     *
     * @param other
     */
    ModuleObjectReference(const ModuleObjectReference &other);

    /**
     * Default destructor
     *
     * In the ModuleObjectFactory when test is done on the m_numberOfReference member, if it reached 0 (zero) then it means that there is no longer reference to the m_pModuleObjectPointer member.
     * So the current ModuleObjectReference in the ModuleFactory map is deleted and the destructor of ModuleObjectReference is called.
     * And object pointed by m_pModuleObjectPointer is removed from the memory (see destructor implementation).
     */
    virtual ~ModuleObjectReference();

    /**
     * Increment by one the number of reference to the pointed object m_pModuleObjectPointer;
     */
    void incCounter();

    /**
     * Decrement by one the number of reference to the pointed object m_pModuleObjectPointer;
     */
    void decCounter();

    virtual std::string toString() const;

    unsigned int getNumberOfReference() const;
    void setNumberOfReference(unsigned int numberOfReference);
    const ModuleObject* getModuleObjectPointer() const;

private:
    ModuleObject* m_pModuleObjectPointer; ///< Keep pointer to the module object in memory.

    unsigned int m_numberOfReference; ///< Count the number of reference to the pointer member in the all running program.
};

} /* namespace PARTONS */

#endif /* MODULE_OBJECT_REFERENCE_H */
