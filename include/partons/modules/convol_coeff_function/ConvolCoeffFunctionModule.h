#ifndef CONVOL_COEFF_FUNCTION_MODULE_H
#define CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file ConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
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
#include "../gpd/GPDModule.h"
#include "../MathIntegratorModule.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Convolution of Coefficient Function module.
 */
template<typename KinematicType, typename ResultType>
class ConvolCoeffFunctionModule: public ModuleObject,
        public MathIntegratorModule {

public:

    //TODO move to DVCS
    static const std::string CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionModule() {

        if (m_pGPDModule != 0) {
            setGPDModule(0);
            m_pGPDModule = 0;
        }
    }

    virtual ConvolCoeffFunctionModule* clone() const = 0;

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
        configureIntegrator(parameters);
    }

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData) {

        //run for mother
        ModuleObject::prepareSubModules(subModulesData);

        //iterator
        std::map<std::string, BaseObjectData>::const_iterator it;

        //check if GPD module dependent
        if (isGPDModuleDependent()) {

            //search for GPD module
            it = subModulesData.find(GPDModule::GPD_MODULE_CLASS_NAME);

            //check if there
            if (it != subModulesData.end()) {

                //check if already set
                if (m_pGPDModule != 0) {

                    setGPDModule(0);
                    m_pGPDModule = 0;
                }

                //set
                if (m_pGPDModule == 0) {

                    m_pGPDModule =
                            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                                    (it->second).getModuleClassName());

                    info(__func__,
                            ElemUtils::Formatter()
                                    << "Configured with GPDModule = "
                                    << m_pGPDModule->getClassName());

                    m_pGPDModule->configure((it->second).getParameters());
                    m_pGPDModule->prepareSubModules(
                            (it->second).getSubModules());
                }

            } else {

                //throw error
                throw ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter() << getClassName()
                                << " is GPDModule dependent and you have not provided one");
            }
        }
    }

    /**
     * Computes the coefficient functions at given kinematics.
     * @param kinematic Kinematics.
     * @param gpdType Type of CCF to compute.
     * @return Complex result.
     */
    virtual ResultType compute(const KinematicType& kinematic,
            const List<GPDType>& gpdType) = 0;

    /**
     * Must be implemented in child class.
     * @return List of GPD/CCF types the child class can compute.
     */
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const = 0;

    // ##### GETTERS & SETTERS #####

    /**
     * Get pointer to the underlying GPD module.
     */
    GPDModule* getGPDModule() const {
        return m_pGPDModule;
    }

    /**
     * Set pointer to the underlying GPD module.
     */
    void setGPDModule(GPDModule* gpdModule) {

        m_pModuleObjectFactory->updateModulePointerReference(m_pGPDModule,
                gpdModule);
        m_pGPDModule = gpdModule;
    }

    /**
     * True if this CCF module depends on a GPD module.
     */
    bool isGPDModuleDependent() const {
        return m_isGPDModuleDependent;
    }

    /**
     * True if this CCF module depends on a GPD module.
     */
    void setIsGPDModuleDependent(bool isGPDModuleDependent) {
        m_isGPDModuleDependent = isGPDModuleDependent;
    }

protected:

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     * @param channelType Channel type.
     */
    ConvolCoeffFunctionModule(const std::string &className,
            ChannelType::Type channelType) :
            ModuleObject(className, channelType), MathIntegratorModule(), m_isGPDModuleDependent(
                    true), m_pGPDModule(0) {
    }

    /**
     * Copy constructor
     * @param other Object to be copied.
     */
    ConvolCoeffFunctionModule(const ConvolCoeffFunctionModule &other) :
            ModuleObject(other), MathIntegratorModule(other), m_isGPDModuleDependent(
                    other.m_isGPDModuleDependent), m_pGPDModule(0) {

        if (other.m_pGPDModule != 0) {
            m_pGPDModule = (other.m_pGPDModule)->clone();
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
    }

    /**
     * Pointer to the underlying GPD module.
     */
    GPDModule* m_pGPDModule;

private:

    /**
     * Boolean (true if this CCF module depends on a GPD module).
     */
    bool m_isGPDModuleDependent;
};

template<typename KinematicType, typename ResultType>
const std::string ConvolCoeffFunctionModule<KinematicType, ResultType>::CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME =
        "ConvolCoeffFunctionModule";

static const std::string CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME;

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_MODULE_H */
