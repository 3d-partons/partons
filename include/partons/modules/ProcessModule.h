#ifndef PROCESS_MODULE_H
#define PROCESS_MODULE_H

/**
 * @file ProcessModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 19, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/observable/ObservableChannel.h"
#include "../ModuleObject.h"

class ConvolCoeffFunctionModule;
class Observable;
class ScaleModule;
class XiConverterModule;

namespace NumA {
class Vector3D;
} /* namespace NumA */

class Vector3D;

/**
 * @class ProcessModule
 *
 * @brief
 */
class ProcessModule: public ModuleObject {
public:
    static const std::string PARAMETER_NAME_BEAM_ENERGY;

    /**
     * Default constructor
     */
    ProcessModule(const std::string &className);

    virtual ProcessModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~ProcessModule();

    virtual void computeConvolCoeffFunction(double xB, double t, double Q2) = 0;

    virtual double computeCrossSection(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, double phi) = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void setConvolCoeffFunctionModule(
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

    void setBeamEnergy(double EBeam); ///< Sets beam energy

    void setPScaleModule(ScaleModule* pScaleModule);
    void setPXiConverterModule(XiConverterModule* pXiConverterModule);
protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    ProcessModule(const ProcessModule &other);

    //TODO doc
    ObservableChannel::Type m_channel;

    // Invariant scalars
    double m_xB;        ///< Bjorken variable
    double m_t;     ///< Mandelstam variable (square of the 4-momentum transfer)
    double m_Q2;    ///< Virtuality of the photon

    // Frame dependent scalars
    double m_E;     ///< Beam energy in target rest frame

    // Angles in Trento convention
    double m_phi;      ///<  Angle between leptonic and hadronic planes (radian)
    double m_phiS;      ///< Angle of the target transverse polarization
    double m_phie;      //TODO What's this?! It's the equivalent of phiS but for GV. Redundant!

    ScaleModule* m_pScaleModule;
    XiConverterModule* m_pXiConverterModule;

    Observable* m_pObservable;
    ConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule;

    bool isPreviousKinematicsDifferent(double xB, double t, double Q2);

private:

    void resetPreviousKinematics();
};

#endif /* PROCESS_MODULE_H */
