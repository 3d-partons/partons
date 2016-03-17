#ifndef DVCS_MODULE_H
#define DVCS_MODULE_H

/**
 * @file DVCSModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 19 November 2014
 * @version 1.0
 *
 * @class DVCSModule
 *
 * @brief
 */

#include <complex>
#include <string>

#include "../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../beans/gpd/GPDType.h"
#include "../ProcessModule.h"

class DVCSConvolCoeffFunctionModule;
class Observable;
class ScaleModule;
class XiConverterModule;

class DVCSModule: public ProcessModule {
public:
    static const std::string PARAMETER_NAME_BEAM_ENERGY;

    DVCSModule(const std::string &className);
    virtual ~DVCSModule();

    virtual void init();

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    virtual void configure(const ElemUtils::Parameters &parameters);

    void computeConvolCoeffFunction(double xB, double t, double Q2);

    // TODO convert double to integer
    double computeCrossSection(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, double phi);

    DVCSConvolCoeffFunctionModule* getDVCSConvolCoeffFunctionModule() const;
    void setDVCSConvolCoeffFunctionModule(
            DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule);
    void setPScaleModule(ScaleModule* pScaleModule);
    void setPXiConverterModule(XiConverterModule* pXiConverterModule);

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

    void SetBeamEnergy(double EBeam); ///< Sets beam energy

    double m_phi;      ///<  Angle between leptonic and hadronic planes (radian)
    double m_phiS;      ///<
    double m_phie;      ///<

    double m_phaseSpace;

    Observable* m_pObservable;
    DVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult;

    ScaleModule* m_pScaleModule;

    XiConverterModule* m_pXiConverterModule;

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

private:

    DVCSConvolCoeffFunctionModule* m_pDVCSConvolCoeffFunctionModule;

    bool isPreviousKinematicsDifferent(double xB, double t, double Q2);
    void resetPreviousKinematics();
};

#endif /* DVCS_MODULE_H */
