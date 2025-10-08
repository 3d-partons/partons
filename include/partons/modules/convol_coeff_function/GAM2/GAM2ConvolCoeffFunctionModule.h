#ifndef GAM2_CONVOL_COEFF_FUNCTION_MODULE_H
#define GAM2_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file GAM2ConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../../../beans/PolarizationType.h"
#include "../../../utils/type/PhysicalType.h"
#include "../ConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class GAM2ConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Compton Form Factor (CFF) module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class GAM2ConvolCoeffFunctionModule: public ConvolCoeffFunctionModule<
        GAM2ConvolCoeffFunctionKinematic, GAM2ConvolCoeffFunctionResult> {

public:

    static const std::string GAM2_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~GAM2ConvolCoeffFunctionModule();

    virtual GAM2ConvolCoeffFunctionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual GAM2ConvolCoeffFunctionResult compute(
            const GAM2ConvolCoeffFunctionKinematic& kinematic,
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
    GAM2ConvolCoeffFunctionModule(const std::string &className);

    /**
     * Copy constructor.
     *
     * @param other Object to be copied
     */
    GAM2ConvolCoeffFunctionModule(const GAM2ConvolCoeffFunctionModule &other);

    virtual void setKinematics(
            const GAM2ConvolCoeffFunctionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * List of GPD/CFF types the child class can compute.
     */
    std::map<GPDType::Type,
            std::complex<double> (GAM2ConvolCoeffFunctionModule::*)()> m_listOfCFFComputeFunctionAvailable;

    /**
     * Iterator.
     */
    std::map<GPDType::Type,
            std::complex<double> (GAM2ConvolCoeffFunctionModule::*)()>::iterator m_it;

    double m_uPrim; ///< Four-momentum transfer squared to photon (in \f$GeV^{2}\f$).
    double m_Mgg2; ///< Invariant mass of two photons (in \f$GeV^{2}\f$).

    PolarizationType::Type m_polG0; ///< Polarization state of incoming photon.
    PolarizationType::Type m_polG1; ///< Polarization state of first outgoing photon.
    PolarizationType::Type m_polG2; ///< Polarization state of second outgoing photon.

    double m_phi; ///< Linear polarization angle.

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< Order of the perturbative QCD computation.
};

} /* namespace PARTONS */

#endif /* GAM2_CONVOL_COEFF_FUNCTION_MODULE_H */
