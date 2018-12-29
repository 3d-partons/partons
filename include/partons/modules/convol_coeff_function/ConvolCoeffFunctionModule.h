#ifndef CONVOL_COEFF_FUNCTION_MODULE_H
#define CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file ConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/channel/ChannelType.h"
#include "../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../ModuleObject.h"
#include "../MathIntegratorModule.h"

namespace PARTONS {

class GPDModule;

/**
 * @class ConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Convolution of Coefficient Function module.
 * It must be derived into child classes for different channels.
 * E.g. for DVCS, it is derived into DVCSConvolCoeffFunctionModule, that is a Compton Form Factor module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class ConvolCoeffFunctionModule: public ModuleObject,
        public MathIntegratorModule {
public:
    static const std::string CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    ConvolCoeffFunctionModule(const std::string &className);
    /**
     * Default destructor.
     */
    virtual ~ConvolCoeffFunctionModule();

    virtual ConvolCoeffFunctionModule* clone() const = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void run();

    /**
     * Computes the coefficient functions at given kinematics.
     * @param kinematic
     * @param gpdType Type of CCF to compute.
     * @return Complex result.
     */
    virtual std::complex<double> compute(
            const DVCSConvolCoeffFunctionKinematic &kinematic,
            GPDType::Type gpdType);

    /**
     * Computes the coefficient functions at given kinematics.
     * Must be implemented in child class.
     * @param xi Skewness.
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     * @param Q2 Virtuality of the photon in Born approximation (in GeV^2).
     * @param MuF2 Factorization scale (in GeV^2).
     * @param MuR2 Renormalization scale (in GeV^2).
     * @param gpdType Type of CCF to compute.
     * @return Complex result.
     */
    virtual std::complex<double> compute(double xi, double t, double Q2,
            double MuF2, double MuR2, GPDType::Type gpdType) = 0;

    /**
     * Must be implemented in child class.
     * @return List of GPD/CCF types the child class can compute.
     */
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const = 0;

    // ##### GETTERS & SETTERS #####

    /**
     *
     * @return Pointer to the underlying GPD module.
     */
    GPDModule* getGPDModule() const;
    /**
     *
     * @param gpdModule Pointer to the underlying GPD module.
     */
    void setGPDModule(GPDModule* gpdModule);

    /**
     *
     * @return Type of channel (i.e. experimental process).
     */
    ChannelType::Type getChannel() const;
    /**
     *
     * @param channel Type of channel (i.e. experimental process).
     */
    void setChannel(ChannelType::Type channel);
    /**
     *
     * @return Boolean (true if this CCF module depends on a GPD module).
     */
    bool isGPDModuleDependent() const;
    /**
     *
     * @param isGPDModuleDependent Boolean (true if this CCF module depends on a GPD module).
     */
    void setIsGPDModuleDependent(bool isGPDModuleDependent);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:
    /**
     * Copy constructor
     */
    ConvolCoeffFunctionModule(const ConvolCoeffFunctionModule &other);

    //TODO doc
    ChannelType::Type m_channel; ///< Type of channel (i.e. experimental process).

    GPDModule* m_pGPDModule; ///< Pointer to the underlying GPD module.

private:
    bool m_isGPDModuleDependent; ///< Boolean (true if this CCF module depends on a GPD module).
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_MODULE_H */
