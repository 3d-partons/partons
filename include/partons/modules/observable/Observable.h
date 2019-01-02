#ifndef OBSERVABLE_H
#define OBSERVABLE_H

/**
 * @file Observable.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 25, 2014
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <map>
#include <string>
#include <utility>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/channel/ChannelType.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../ModuleObjectFactory.h"
#include "../../Partons.h"
#include "../process/DVCS/DVCSProcessModule.h"
#include "../process/ProcessModule.h"

namespace PARTONS {

/**
 * @class Observable
 *
 * @brief Abstract class that provides a skeleton to implement an Observable module.
 *
 * It is best to use this module with the corresponding service: ObservableService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
template<typename KinematicType, typename ResultType>
class Observable: public ModuleObject {

public:

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     * @param channelType Channel type.
     */
    Observable(const std::string &className, ChannelType::Type channelType) :
            ModuleObject(className, channelType), m_pProcessModule(0) {
    }

    /**
     * Destructor
     */
    virtual ~Observable() {

        if (m_pProcessModule != 0) {
            setProcessModule(0);
            m_pProcessModule = 0;
        }
    }

    virtual Observable* clone() const = 0;

    virtual std::string toString() const {
        return ModuleObject::toString();
    }

    virtual void resolveObjectDependencies() {
        ModuleObject::resolveObjectDependencies();
    }

    virtual void run() {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "This must be implemented in daughter class");
    }

    virtual void configure(const ElemUtils::Parameters &parameters) {
        ModuleObject::configure(parameters);
    }

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData) {

        //run for mother
        ModuleObject::prepareSubModules(subModulesData);

        //iterator
        std::map<std::string, BaseObjectData>::const_iterator it;

        //search for GPD module
        it = subModulesData.find(DVCSProcessModule::PROCESS_MODULE_CLASS_NAME);

        //check if there
        if (it != subModulesData.end()) {

            //check if already set
            if (m_pProcessModule) {

                setProcessModule(0);
                m_pProcessModule = 0;
            }

            //set
            if (!m_pProcessModule) {

                m_pProcessModule =
                        Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                                (it->second).getModuleClassName());

                info(__func__,
                        ElemUtils::Formatter()
                                << "Configured with ProcessModule = "
                                << m_pProcessModule->getClassName());

                m_pProcessModule->configure((it->second).getParameters());
                m_pProcessModule->prepareSubModules(
                        (it->second).getSubModules());
            } else {

                //throw error
                throw ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter() << getClassName()
                                << " is DVCSProcessModule dependent and you have not provided one");
            }
        }
    }

    /**
     * Computes the observable at given kinematics.
     * @param kinematic Kinematics.
     * @param gpdType Type of GPDs to compute.
     * @return Result.
     */
    virtual ResultType compute(const KinematicType& kinematic,
            const List<GPDType> & gpdType = List<GPDType>()) = 0;

    // ##### GETTERS & SETTERS #####

    DVCSProcessModule* getProcessModule() const {
        return m_pProcessModule;
    }

    void setProcessModule(DVCSProcessModule* pProcessModule) {

        m_pModuleObjectFactory->updateModulePointerReference(m_pProcessModule,
                pProcessModule);
        m_pProcessModule = pProcessModule;

        if (m_pProcessModule != 0) {
            info(__func__,
                    ElemUtils::Formatter() << "ProcessModule is set to: "
                            << m_pProcessModule->getClassName());
        } else {
            info(__func__, "ProcessModule is set to: 0");
        }
    }

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Observable(const Observable& other) :
            ModuleObject(other) {

        if (other.m_pProcessModule != 0) {
            m_pProcessModule = other.m_pProcessModule->clone();
        } else {
            m_pProcessModule = 0;
        }
    }

    /**
     * Set internal kinematics
     * @param kinematic Kinematics to be set
     */
    virtual void setKinematics(const KinematicType& kinematic) = 0;

    virtual void initModule() {
    }

    virtual void isModuleWellConfigured() {

        //check if pointer to process module set
        if (m_pProcessModule == 0) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "m_pProcessModule is NULL pointer ; Use configure method to configure it");
        }
    }

    /**
     * Pointer to DVCS process module.
     */
    DVCSProcessModule* m_pProcessModule;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_H */
