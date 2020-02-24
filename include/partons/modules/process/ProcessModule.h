#ifndef PROCESS_MODULE_H
#define PROCESS_MODULE_H

/**
 * @file ProcessModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 19, 2014
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/channel/ChannelType.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../ModuleObject.h"

namespace NumA {
class Vector3D;
} /* namespace NumA */

namespace PARTONS {

/**
 * @class ProcessModule
 *
 * @brief Abstract class that provides a skeleton to implement a Process module.
 */
template<typename KinematicType, typename ResultType>
class ProcessModule: public ModuleObject {

public:

    /**
     * Destructor.
     */
    virtual ~ProcessModule() {
    }

    virtual ProcessModule* clone() const = 0;

    virtual std::string toString() const {
        return ModuleObject::toString();
    }

    virtual void resolveObjectDependencies() {
        ModuleObject::resolveObjectDependencies();
    }

    virtual void run() {
        throw ElemUtils::CustomException("Thread", __func__,
                "This must be implemented in daughter class");
    }

    virtual void configure(const ElemUtils::Parameters &parameters) {
        ModuleObject::configure(parameters);
    }

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData) {
        ModuleObject::prepareSubModules(subModulesData);
    }

    /**
     * Must be implemented in child class.
     * @return List of GPD/CCF types the child class can compute.
     */
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const = 0;

    // ##### GETTERS & SETTERS #####

    /**
     * Check if this process module depends on a CCF module.
     */
    bool isCCFModuleDependent() const {
        return m_isCCFModuleDependent;
    }

    /**
     * Set if this process module depends on a CCF module.
     */
    void setIsCCFModuleDependent(bool isCCFModuleDependent) {
        m_isCCFModuleDependent = isCCFModuleDependent;
    }

protected:

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     * @param channelType Channel type.
     */
    ProcessModule(const std::string &className, ChannelType::Type channelType) :
            ModuleObject(className, channelType), m_isCCFModuleDependent(true) {
    }

    /**
     * Copy constructor
     * @param other Object to be copied.
     */
    ProcessModule(const ProcessModule &other) :
            ModuleObject(other), m_isCCFModuleDependent(
                    other.m_isCCFModuleDependent) {

    }

    /**
     * Set internal kinematics.
     * @param kinematic Kinematics to be set
     */
    virtual void setKinematics(const KinematicType& kinematic) = 0;

    virtual void initModule() {
    }

    virtual void isModuleWellConfigured() {
    }

    /**
     * Boolean (true if this Process module depends on a CCF module).
     */
    bool m_isCCFModuleDependent;
};

} /* namespace PARTONS */

#endif /* PROCESS_MODULE_H */
