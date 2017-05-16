#ifndef PROCESS_MODULE_H
#define PROCESS_MODULE_H

/**
 * @file ProcessModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 19, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../beans/automation/BaseObjectData.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../beans/observable/ObservableChannel.h"
#include "../ModuleObject.h"

class ConvolCoeffFunctionModule;
class ScaleModule;
class XiConverterModule;

namespace NumA {
class Vector3D;
} /* namespace NumA */

class Vector3D;

/**
 * @class ProcessModule
 *
 * @brief Generic class for computing the cross section of an experimental process.
 * The different channels are child classes, e.g. DVCSModule.
 */
class ProcessModule: public ModuleObject {
public:
    static const std::string PROCESS_MODULE_CLASS_NAME;

    /**
     * Default constructor.
     */
    ProcessModule(const std::string &className);

    virtual ProcessModule* clone() const = 0;

    /**
     * Default destructor.
     */
    virtual ~ProcessModule();

    /**
     * Computes the coefficient functions associated to the process.
     * Must be implemented in the child class, by calling a ConvolCoeffFunctionModule.
     * @param xB Bjorken x variable.
     * @param t Momentum transfer squared.
     * @param Q2 Virtuality.
     * @param E Beam energy.
     * @param gpdType List of GPD types to be used for the coefficient functions.
     */
    virtual void computeConvolCoeffFunction(double xB, double t, double Q2,
            double E, const List<GPDType> & gpdType = List<GPDType>()) = 0;

    /**
     * Computes the cross section. Must be implemented in the child class.
     * @param beamHelicity Helicity of the beam.
     * @param beamCharge Charge of the beam.
     * @param targetPolarization Polarization of the target.
     * @param phi Angle between the hadronic and leptonic planes.
     * @return
     */
    virtual double computeCrossSection(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, double phi) = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Sets a given ConvolCoeffFunctionModule.
     * @param pConvolCoeffFunctionModule
     */
    virtual void setConvolCoeffFunctionModule(
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

    void setScaleModule(ScaleModule* pScaleModule);
    void setXiConverterModule(XiConverterModule* pXiConverterModule);
    bool isCCFModuleDependent() const;
    void isCCFModuleDependent(bool isCcfModuleDependent);

    ConvolCoeffFunctionModule* getConvolCoeffFunctionModule() const;

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    void resetPreviousKinematics();

protected:
    /**
     * Copy constructor.
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
    double m_phie; //TODO What's this?! It's the equivalent of phiS but for GV. Redundant! TODO: Remove it!

    ScaleModule* m_pScaleModule;
    XiConverterModule* m_pXiConverterModule;

    ConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule;

    bool isPreviousKinematicsDifferent(double xB, double t, double Q2);

private:
    bool m_isCCFModuleDependent;
};

#endif /* PROCESS_MODULE_H */
