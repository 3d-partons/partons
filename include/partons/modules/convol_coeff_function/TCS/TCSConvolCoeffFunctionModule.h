#ifndef TCS_CONVOL_COEFF_FUNCTION_MODULE_H
#define TCS_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file TCSConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../ConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class TCSConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Compton Form Factor (CFF) module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class TCSConvolCoeffFunctionModule: public ConvolCoeffFunctionModule<
        TCSConvolCoeffFunctionKinematic, TCSConvolCoeffFunctionResult> {

public:

    static const std::string TCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~TCSConvolCoeffFunctionModule();

    virtual TCSConvolCoeffFunctionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual TCSConvolCoeffFunctionResult compute(
            const TCSConvolCoeffFunctionKinematic& kinematic,
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
    TCSConvolCoeffFunctionModule(const std::string &className);

    /**
     * Copy constructor.
     *
     * @param other Object to be copied
     */
    TCSConvolCoeffFunctionModule(const TCSConvolCoeffFunctionModule &other);

    virtual void setKinematics(
            const TCSConvolCoeffFunctionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * List of GPD/CFF types the child class can compute.
     */
    std::map<GPDType::Type,
            std::complex<double> (TCSConvolCoeffFunctionModule::*)()> m_listOfCFFComputeFunctionAvailable;

    /**
     * Iterator.
     */
    std::map<GPDType::Type,
            std::complex<double> (TCSConvolCoeffFunctionModule::*)()>::iterator m_it;

    double m_Q2Prim; ///< Virtuality of the photon (in GeV^2).

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< Order of the perturbative QCD computation.
};

} /* namespace PARTONS */

#endif /* TCS_CONVOL_COEFF_FUNCTION_MODULE_H */
