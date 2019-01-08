#ifndef DVCS_CONVOL_COEFF_FUNCTION_MODULE_H
#define DVCS_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file DVCSConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../ConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Compton Form Factor (CFF) module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class DVCSConvolCoeffFunctionModule: public ConvolCoeffFunctionModule<
        DVCSConvolCoeffFunctionKinematic, DVCSConvolCoeffFunctionResult> {

public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionModule();

    virtual DVCSConvolCoeffFunctionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual DVCSConvolCoeffFunctionResult compute(
            const DVCSConvolCoeffFunctionKinematic& kinematic,
            const List<GPDType>& gpdType);
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

protected:

    /**
     * Copy constructor.
     *
     * @param other Object to be copied
     */
    DVCSConvolCoeffFunctionModule(const DVCSConvolCoeffFunctionModule &other);

    virtual void setKinematics(
            const DVCSConvolCoeffFunctionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

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

    /**
     * List of GPD/CFF types the child class can compute.
     */
    std::map<GPDType::Type,
            std::complex<double> (DVCSConvolCoeffFunctionModule::*)()> m_listOfCFFComputeFunctionAvailable;

    /**
     * Iterator.
     */
    std::map<GPDType::Type,
            std::complex<double> (DVCSConvolCoeffFunctionModule::*)()>::iterator m_it;

    double m_Q2; ///< Virtuality of the photon (in GeV^2).

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< Order of the perturbative QCD computation.
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_MODULE_H */
