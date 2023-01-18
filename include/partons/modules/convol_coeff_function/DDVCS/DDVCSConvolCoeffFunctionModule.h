#ifndef DDVCS_CONVOL_COEFF_FUNCTION_MODULE_H
#define DDVCS_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file DDVCSConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../ConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Compton Form Factor (CFF) module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class DDVCSConvolCoeffFunctionModule: public ConvolCoeffFunctionModule<
        DDVCSConvolCoeffFunctionKinematic, DDVCSConvolCoeffFunctionResult> {

public:

    static const std::string DDVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionModule();

    virtual DDVCSConvolCoeffFunctionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual DDVCSConvolCoeffFunctionResult compute(
            const DDVCSConvolCoeffFunctionKinematic& kinematic,
            const List<GPDType>& gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get order of the perturbative QCD computation.
     */
    PerturbativeQCDOrderType::Type getQCDOrderType() const;

    /**
     * Set order of the perturbative QCD computation.
     */
    void setQCDOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

    // #### IMPLEMENTATION MEMBERS ####

    /**
     * Method to compute some CFFs.
     * Can be implemented and used in child class, if needed.
     * @return Complex result.
     */
    virtual std::complex<double> computeUnpolarized();

    /**
     * Method to compute some CFFs.
     * Can be implemented and used in child class, if needed.
     * @return Complex result.
     */
    virtual std::complex<double> computePolarized();

    /**
     * Method to compute some CFFs.
     * Can be implemented and used in child class, if needed.
     * @return Complex result.
     */
    virtual std::complex<double> computeCFF();

protected:

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionModule(const std::string &className);

    /**
     * Copy constructor.
     *
     * @param other Object to be copied
     */
    DDVCSConvolCoeffFunctionModule(const DDVCSConvolCoeffFunctionModule &other);

    virtual void setKinematics(
            const DDVCSConvolCoeffFunctionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * List of GPD/CFF types the child class can compute.
     */
    std::map<GPDType::Type,
            std::complex<double> (DDVCSConvolCoeffFunctionModule::*)()> m_listOfCFFComputeFunctionAvailable;

    /**
     * Iterator.
     */
    std::map<GPDType::Type,
            std::complex<double> (DDVCSConvolCoeffFunctionModule::*)()>::iterator m_it;

    double m_Q2; ///< Virtuality of the photon (in GeV^2).
    double m_Q2Prim; ///< Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).

    double m_rho; ///< rho.

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< Order of the perturbative QCD computation.
};

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_MODULE_H */
