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
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../ConvolCoeffFunctionModule.h"

namespace PARTONS {

class ActiveFlavorsThresholdsModule;
class DVCSConvolCoeffFunctionResult;
class RunningAlphaStrongModule;

/**
 * @class DVCSConvolCoeffFunctionModule
 *
 * @brief Abstract class that provides a skeleton to implement a Compton Form Factor (CFF) module.
 *
 * It is best to use this module with the corresponding service: ConvolCoeffFunctionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class DVCSConvolCoeffFunctionModule: public ConvolCoeffFunctionModule<
        DVCSConvolCoeffFunctionKinematic> {

public:

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className Name of child class.
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

    virtual std::complex<double> compute(
            const DVCSConvolCoeffFunctionKinematic& kinematic,
            GPDType::Type gpdType);

    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    /**
     * Computes the coefficient functions at given kinematics.
     * @param xi Skewness.
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     * @param Q2 Virtuality of the photon (in GeV^2).
     * @param MuF2 Factorization scale (in GeV^2).
     * @param MuR2 Renormalization scale (in GeV^2)
     * @param gpdType GPDType of the CFF computation.
     */
    virtual std::complex<double> compute(double xi, double t, double Q2,
            double MuF2, double MuR2, GPDType::Type gpdType);

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

    // ##### GETTERS & SETTERS #####

    /**
     * Get order of the perturbative QCD computation.
     */
    PerturbativeQCDOrderType::Type getQCDOrderType() const;

    /**
     * Set order of the perturbative QCD computation.
     */
    void setQCDOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

    //TO BE REMOVED
    void setNfConvolCoeffFunction(
            ActiveFlavorsThresholdsModule* pNfConvolCoeffFunction);

    //TO BE REMOVED
    void setRunningAlphaStrongModule(
            RunningAlphaStrongModule* pRunningAlphaStrongModule);

protected:

    /**
     * Copy constructor.
     *
     * @param other Object to be copied
     */
    DVCSConvolCoeffFunctionModule(const DVCSConvolCoeffFunctionModule &other);

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

    double m_xi; ///< Skewness.
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
    double m_Q2; ///< Virtuality of the photon (in GeV^2).
    double m_MuF2; ///< Factorization scale (in GeV^2).
    double m_MuR2; ///< Renormalization scale (in GeV^2)

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< Order of the perturbative QCD computation.
    GPDType::Type m_currentGPDComputeType; ///< GPDType of the current CFF computation.

    //TO BE REMOVED
    unsigned int m_nf; ///< Number of flavors.
    RunningAlphaStrongModule* m_pRunningAlphaStrongModule; ///< Pointer to the running coupling module to be used.
    ActiveFlavorsThresholdsModule* m_pNfConvolCoeffFunction;

    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * Method called at the start of the compute method to test the input.
     * Calls initModule and isModuleWellConfigured.
     * @param xi Skewness.
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2).
     * @param Q2 Virtuality of the photon (in GeV^2).
     * @param MuF2 Factorization scale (in GeV^2).
     * @param MuR2 Renormalization scale (in GeV^2)
     * @param gpdType GPDType of the CFF computation.
     */
    void preCompute(double xi, double t, double Q2, double MuF2, double MuR2,
            GPDType::Type gpdType);
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_MODULE_H */
