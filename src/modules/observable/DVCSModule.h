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

class Vector3D;
class Observable;

class DVCSModule: public ObservableModule {
public:
    DVCSModule(const std::string &className);
    virtual ~DVCSModule();

    virtual double computeWithPhiDependency(double xB, double t, double Q2,
            double phi,
            DVCSConvolCoeffFunctionResult dvcsConvolCoeffFunctionResult,
            Observable* pObservable) = 0;

    virtual double computeCrossSection(double beamHelicity, double beamCharge,
            Vector3D targetPolarization) = 0;

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    DVCSModule(const DVCSModule& other);

    double m_xB;        ///< Bjorken variable
    double m_t;     ///< Mandelstam variable (square of the 4-momentum transfer)
    double m_Q2;    ///< Virtuality of the photon

    double m_phi;      ///<  Angle between leptonic and hadronic planes (radian)
    double m_phiS;      ///<
    double m_phie;      ///<

    double m_E;     ///< Beam energy in target rest frame

    double m_phaseSpace;

    Observable* m_pObservable;
    DVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult;

private:
};

#endif /* DVCS_MODULE_H */
