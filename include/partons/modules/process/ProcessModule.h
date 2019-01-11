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
#include "../scales/ScalesModule.h"
#include "../xi_converter/XiConverterModule.h"

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

        if (m_pScaleModule != 0) {
            setScaleModule(0);
            m_pScaleModule = 0;
        }

        if (m_pXiConverterModule != 0) {
            setXiConverterModule(0);
            m_pXiConverterModule = 0;
        }
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

        //run for mother class
        ModuleObject::prepareSubModules(subModulesData);

        //iterator
        std::map<std::string, BaseObjectData>::const_iterator it;

        //search for scales module
        it = subModulesData.find(ScalesModule::SCALES_MODULE_CLASS_NAME);

        //check if there
        if (it != subModulesData.end()) {

            //check if already set
            if (m_pScaleModule) {

                setScaleModule(0);
                m_pScaleModule = 0;
            }

            //set
            if (!m_pScaleModule) {

                m_pScaleModule =
                        Partons::getInstance()->getModuleObjectFactory()->newScalesModule(
                                (it->second).getModuleClassName());

                info(__func__,
                        ElemUtils::Formatter()
                                << "Configured with ScaleModule = "
                                << m_pScaleModule->getClassName());

                m_pScaleModule->configure((it->second).getParameters());
                m_pScaleModule->prepareSubModules((it->second).getSubModules());
            }

        } else {

            //throw error
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << getClassName()
                            << " is ScaleModule dependent and you have not provided one");
        }

        //search for xi module
        it = subModulesData.find(
                XiConverterModule::XI_CONVERTER_MODULE_CLASS_NAME);

        //check if there
        if (it != subModulesData.end()) {

            //check if already set
            if (m_pXiConverterModule) {

                setXiConverterModule(0);
                m_pXiConverterModule = 0;
            }

            //set
            if (!m_pXiConverterModule) {

                m_pXiConverterModule =
                        Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(
                                (it->second).getModuleClassName());

                info(__func__,
                        ElemUtils::Formatter()
                                << "Configured with XiConverterModule = "
                                << m_pXiConverterModule->getClassName());

                m_pXiConverterModule->configure((it->second).getParameters());
                m_pXiConverterModule->prepareSubModules(
                        (it->second).getSubModules());
            }

        } else {

            //throw error
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << getClassName()
                            << " is XiConverterModule dependent and you have not provided one");
        }

    }

    /**
     * Computes the differential cross-section. Must be implemented in the child class.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Charge of the beam (in units of positron charge).
     * @param targetPolarization Polarization of the target. In GV conventions.
     * @param kinematic Kinematics.
     * @return Result.
     */
    virtual ResultType compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, const KinematicType& kinematic,
            const List<GPDType>& gpdType = List<GPDType>()) = 0;

    /**
     * Must be implemented in child class.
     * @return List of GPD/CCF types the child class can compute.
     */
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const = 0;

    // ##### GETTERS & SETTERS #####

    /**
     * Get scale module.
     */
    ScalesModule* getScaleModule() const {
        return m_pScaleModule;
    }

    /**
     * Set scale module.
     */
    void setScaleModule(ScalesModule* pScaleModule) {

        m_pModuleObjectFactory->updateModulePointerReference(m_pScaleModule,
                pScaleModule);
        m_pScaleModule = pScaleModule;

        if (m_pScaleModule != 0) {
            info(__func__,
                    ElemUtils::Formatter() << "ScalesModule is set to: "
                            << pScaleModule->getClassName());
        } else {
            info(__func__, "ScalesModule is set to: 0");
        }
    }

    /**
     * Get xi converter module.
     */
    XiConverterModule* getXiConverterModule() const {
        return m_pXiConverterModule;
    }

    /**
     * Set xi converted module.
     */
    void setXiConverterModule(XiConverterModule* pXiConverterModule) {

        m_pModuleObjectFactory->updateModulePointerReference(
                m_pXiConverterModule, pXiConverterModule);
        m_pXiConverterModule = pXiConverterModule;

        if (m_pXiConverterModule != 0) {
            info(__func__,
                    ElemUtils::Formatter() << "XiConverterModule is set to: "
                            << pXiConverterModule->getClassName());
        } else {
            info(__func__, "XiConverterModule is set to: 0");
        }
    }

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
            ModuleObject(className, channelType), m_isCCFModuleDependent(true), m_pScaleModule(
                    0), m_pXiConverterModule(0) {
    }

    /**
     * Copy constructor
     * @param other Object to be copied.
     */
    ProcessModule(const ProcessModule &other) :
            ModuleObject(other), m_isCCFModuleDependent(
                    other.m_isCCFModuleDependent), m_pScaleModule(0), m_pXiConverterModule(
                    0) {

        if (other.m_pScaleModule != 0) {
            m_pScaleModule = m_pModuleObjectFactory->cloneModuleObject(
                    other.m_pScaleModule);
        }

        if (other.m_pXiConverterModule != 0) {
            m_pXiConverterModule = m_pModuleObjectFactory->cloneModuleObject(
                    other.m_pXiConverterModule);
        }
    }

    /**
     * Set internal kinematics.
     * @param kinematic Kinematics to be set
     */
    virtual void setKinematics(const KinematicType& kinematic) = 0;

    /**
     * Set experimental conditions.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Charge of the beam (in units of positron charge).
     * @param targetPolarization Polarization of the target. In GV conventions.
     */
    virtual void setExperimentalConditions(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization) = 0;

    virtual void initModule() {
    }

    virtual void isModuleWellConfigured() {

        //check if pointer to scale module set
        if (m_pScaleModule == 0) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "m_pScaleModule is NULL pointer ; Use configure method to configure it");
        }

        //check if pointer to xi module set
        if (m_pXiConverterModule == 0) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    "m_pXiConverterModule is NULL pointer ; Use configure method to configure it");
        }
    }

    /**
     * Pointer to the underlying scale module.
     */
    ScalesModule* m_pScaleModule;

    /**
     * Pointer to the underlying xi converter module.
     */
    XiConverterModule* m_pXiConverterModule;

    /**
     * Boolean (true if this Process module depends on a CCF module).
     */
    bool m_isCCFModuleDependent;
};

} /* namespace PARTONS */

#endif /* PROCESS_MODULE_H */
