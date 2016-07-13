#ifndef DVCS_MODULE_H
#define DVCS_MODULE_H

/**
 * @file DVCSModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 19, 2014
 * @version 1.0
 */

#include <complex>
#include <string>

#include "../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../beans/gpd/GPDType.h"
#include "../ProcessModule.h"

/**
 * @class DVCSModule
 *
 * @brief
 */
class DVCSModule: public ProcessModule {
public:
    DVCSModule(const std::string &className);
    virtual ~DVCSModule();

    virtual void resolveObjectDependencies();

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    virtual void configure(const ElemUtils::Parameters &parameters);

    void computeConvolCoeffFunction(double xB, double t, double Q2,
            const GPDType::Type gpdType = GPDType::ALL);

    // TODO convert double to integer
    double computeCrossSection(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, double phi);

    virtual void setConvolCoeffFunctionModule(
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    DVCSModule(const DVCSModule& other);

    virtual void initModule();
    virtual void initModule(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual void isModuleWellConfigured();

    double m_phaseSpace;
    double m_tmin; ///< minimum of t
    double m_tmax; ///< max of t
    double m_xBmin; ///< minimum of xB
    double m_y; ///< Lepton energy fraction
    double m_epsilon; ///<

    DVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult;

    // Cross sections
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;
    // Bethe Heitler cross section
    // beamCharge in units of positron charge
    // beamHelicity in units of hbar/2

    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;
    // Virtual Compton Scattering cross section

    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;
    // Interference cross section

    std::complex<double> getConvolCoeffFunctionValue(GPDType::Type gpdType);
};

#endif /* DVCS_MODULE_H */
