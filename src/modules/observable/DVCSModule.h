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

#include <string>

#include "../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../ObservableModule.h"

class DVCSConvolCoeffFunctionModule;

class Observable;
class Vector3D;

class DVCSModule: public ObservableModule {
public:
    DVCSModule(const std::string &className);
    virtual ~DVCSModule();

    void computeConvolCoeffFunction(double xB, double t, double Q2, double MuF2,
            double MuR2);

    // TODO convert double to integer
    double computeCrossSection(double beamHelicity, double beamCharge,
            Vector3D targetPolarization, double phi);

    DVCSConvolCoeffFunctionModule* getDVCSConvolCoeffFunctionModule() const;
    void setDVCSConvolCoeffFunctionModule(
            DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule);

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
            Vector3D targetPolarization);
    virtual void isModuleWellConfigured();


    void SetBeamEnergy(double EBeam); ///< Sets beam energy

    double m_xB;        ///< Bjorken variable
    double m_t;     ///< Mandelstam variable (square of the 4-momentum transfer)
    double m_Q2;    ///< Virtuality of the photon

    double m_MuF2;
    double m_MuR2;

    double m_phi;      ///<  Angle between leptonic and hadronic planes (radian)
    double m_phiS;      ///<
    double m_phie;      ///<

    double m_E;     ///< Beam energy in target rest frame

    double m_phaseSpace;

    Observable* m_pObservable;
    DVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult;

    // Cross sections
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            Vector3D targetPolarization) = 0;
    // Bethe Heitler cross section
    // beamCharge in units of positron charge
    // beamHelicity in units of hbar/2

    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            Vector3D targetPolarization) = 0;
    // Virtual Compton Scattering cross section

    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            Vector3D targetPolarization) = 0;
    // Interference cross section

private:

    DVCSConvolCoeffFunctionModule* m_pDVCSConvolCoeffFunctionModule;

    bool isPreviousKinematicDifferent(double xB, double t, double Q2);
    double xBToXi(double xB);
};

#endif /* DVCS_MODULE_H */
