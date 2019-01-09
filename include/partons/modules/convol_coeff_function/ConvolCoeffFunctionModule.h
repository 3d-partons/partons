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
            ModuleObject(className, channelType), MathIntegratorModule(), m_xi(
                    0.), m_t(0.), m_MuF2(0.), m_MuR2(0.), m_currentGPDComputeType(
                    GPDType::UNDEFINED), m_pGPDModule(0), m_isGPDModuleDependent(
                    true) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ConvolCoeffFunctionModule(const ConvolCoeffFunctionModule &other) :
            ModuleObject(other), MathIntegratorModule(other), m_xi(other.m_xi), m_t(
                    other.m_t), m_MuF2(other.m_MuF2), m_MuR2(other.m_MuR2), m_currentGPDComputeType(
                    other.m_currentGPDComputeType), m_pGPDModule(0), m_isGPDModuleDependent(
                    other.m_isGPDModuleDependent) {

        if (other.m_pGPDModule != 0) {
            m_pGPDModule = (other.m_pGPDModule)->clone();
        }
    }

    /**
     * Set internal kinematics.
     * @param kinematic Kinematics to be set.
     */
    virtual void setKinematics(const KinematicType& kinematic) {

        m_xi = kinematic.getXi().getValue();
        m_t = kinematic.getT().getValue();
        m_MuF2 = kinematic.getMuF2().getValue();
        m_MuR2 = kinematic.getMuR2().getValue();
    }

    /**
     * Set current GPD type to be computed.
     */
    void setCurrentGPDType(GPDType::Type gpdType) {
        m_currentGPDComputeType = gpdType;
    }

    virtual void initModule() {
    }

    virtual void isModuleWellConfigured() {

        if (m_xi < 0. || m_xi > 1.) {
            warn(__func__,
                    ElemUtils::Formatter() << "Input value of Xi = " << m_xi
                            << " do not lay between 0 and 1.");
        }

        if (m_t > 0) {
            warn(__func__,
                    ElemUtils::Formatter() << " Input value of t = " << m_t
                            << " is not <= 0.");
        }

        if (m_MuF2 < 0) {
            warn(__func__,
                    ElemUtils::Formatter() << "Input value of muF2 = " << m_MuF2
                            << " is not > 0.");
        }

        if (m_MuR2 < 0) {
            warn(__func__,
                    ElemUtils::Formatter() << "Input value of muR2 = " << m_MuR2
                            << " is not > 0.");
        }

        if (isGPDModuleDependent() && m_pGPDModule == 0) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    "m_pGPDModule is NULL");
        }
    }

    double m_xi; ///< Skewness.
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
    double m_MuF2; ///< Factorization scale (in GeV^2).
    double m_MuR2; ///< Renormalization scale (in GeV^2)

    GPDType::Type m_currentGPDComputeType; ///< GPDType of the current CFF computation.

    GPDModule* m_pGPDModule; ///< Pointer to the underlying GPD module.

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
