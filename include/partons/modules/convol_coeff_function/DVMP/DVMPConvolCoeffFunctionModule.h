#ifndef DVMP_CONVOL_COEFF_FUNCTION_MODULE_H
#define DVMP_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file DVMPConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../ConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Compton Form Factor (CFF) module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class DVMPConvolCoeffFunctionModule: public ConvolCoeffFunctionModule<
        DVMPConvolCoeffFunctionKinematic, DVMPConvolCoeffFunctionResult> {

public:

    static const std::string DVMP_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionModule();

    virtual DVMPConvolCoeffFunctionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual DVMPConvolCoeffFunctionResult compute(
            const DVMPConvolCoeffFunctionKinematic& kinematic,
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

    virtual std::vector<double> test();

protected:

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionModule(const std::string &className);

    /**
     * Copy constructor.
     *
     * @param other Object to be copied
     */
    DVMPConvolCoeffFunctionModule(const DVMPConvolCoeffFunctionModule &other);

    virtual void setKinematics(
            const DVMPConvolCoeffFunctionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * List of GPD/CFF types the child class can compute.
     */
    std::map<GPDType::Type,
            std::complex<double> (DVMPConvolCoeffFunctionModule::*)()> m_listOfCFFComputeFunctionAvailable;

    /**
     * Iterator.
     */
    std::map<GPDType::Type,
            std::complex<double> (DVMPConvolCoeffFunctionModule::*)()>::iterator m_it;

    double m_Q2; ///< Virtuality of the photon (in GeV^2).
    MesonType::Type m_mesonType; ///< Meson type.
    PolarizationType::Type m_mesonPolarization; ///< Meson polarization state.

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< Order of the perturbative QCD computation.
};

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_MODULE_H */
