#ifndef JET_CONVOL_COEFF_FUNCTION_MODULE_H
#define JET_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file JETConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/JET/JETConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/JET/JETConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../ConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class JETConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Compton Form Factor (CFF) module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class JETConvolCoeffFunctionModule: public ConvolCoeffFunctionModule<
        JETConvolCoeffFunctionKinematic, JETConvolCoeffFunctionResult> {

public:

    static const std::string JET_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~JETConvolCoeffFunctionModule();

    virtual JETConvolCoeffFunctionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual JETConvolCoeffFunctionResult compute(
            const JETConvolCoeffFunctionKinematic& kinematic,
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
    JETConvolCoeffFunctionModule(const std::string &className);

    /**
     * Copy constructor.
     *
     * @param other Object to be copied
     */
    JETConvolCoeffFunctionModule(const JETConvolCoeffFunctionModule &other);

    virtual void setKinematics(
            const JETConvolCoeffFunctionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * List of GPD/CFF types the child class can compute.
     */
    std::map<GPDType::Type,
            std::complex<double> (JETConvolCoeffFunctionModule::*)()> m_listOfCFFComputeFunctionAvailable;

    /**
     * Iterator.
     */
    std::map<GPDType::Type,
            std::complex<double> (JETConvolCoeffFunctionModule::*)()>::iterator m_it;

    double m_z; ///< Longitudinal "minus” momentum fraction carried by the parton forming the jet.
    double m_qPerp2; ///< Square of transverse momentum of parton forming the jet (in  GeV^2$).
    double m_Q2; ///< Virtuality of the photon (in GeV^2).
    JetType::Type m_jetType; ///< Jet type.

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< Order of the perturbative QCD computation.
};

} /* namespace PARTONS */

#endif /* JET_CONVOL_COEFF_FUNCTION_MODULE_H */
