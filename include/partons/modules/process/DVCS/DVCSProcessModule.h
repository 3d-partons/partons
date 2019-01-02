#ifndef DVCS_PROCESS_MODULE_H
#define DVCS_PROCESS_MODULE_H

/**
 * @file DVCSProcessModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 19, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../beans/process/DVCSSubProcessType.h"
#include "../ProcessModule.h"

namespace NumA {
class Vector3D;
} /* namespace NumA */

namespace PARTONS {

/**
 * @class DVCSProcessModule
 *
 * @brief Abstract class for computing the *differential* cross section of
 * the photon electroproduction process (also called DVCS; Deeply Virtual Compton Scattering).
 *
 * The cross-section is five-fold differential with respect to the variables: @f$ x_B @f$, @f$ Q^2 @f$, @f$ t @f$ and the two angles.
 */
class DVCSProcessModule: public ProcessModule<DVCSObservableKinematic,
        DVCSConvolCoeffFunctionKinematic> {

public:

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVCSProcessModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSProcessModule();

    virtual DVCSProcessModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    virtual double compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            const DVCSObservableKinematic& kinematic);
    virtual bool isPreviousCCFKinematicsDifferent(
            const DVCSConvolCoeffFunctionKinematic& kinematic);
    virtual void resetPreviousKinematics();

    /**
     * Computes the differential cross-section. Must be implemented in the child class.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Charge of the beam (in units of positron charge).
     * @param targetPolarization Polarization of the target. In GV conventions.
     * @param kinematic Kinematics.
     * @param processType Subprocess type.
     * @return Result.
     */
    double compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            const DVCSObservableKinematic& kinematic,
            DVCSSubProcessType::Type processType);

    /**
     * Compute CCF for a given kinematics.
     * @param kinematic Kinematics to be computed.
     * @param gpdType List of GPD types to be computed.
     */
    void computeConvolCoeffFunction(const DVCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());

    // ##### GETTERS & SETTERS #####

    /**
     * Get CCF module;
     */
    DVCSConvolCoeffFunctionModule* getConvolCoeffFunctionModule() const;

    /**
     * Set CCF module
     */
    void setConvolCoeffFunctionModule(
            DVCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSProcessModule(const DVCSProcessModule& other);

    virtual void initModule();

    virtual void isModuleWellConfigured();

    /**
     * Method called at the start of the compute method to test the input.
     * Calls initModule and isModuleWellConfigured.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Charge of the beam (in units of positron charge).
     * @param targetPolarization Polarization of the target. In GV conventions.
     * @param xB Bjorken variable.
     *  @param t Mandelstam variable (square of the 4-momentum transferm in GeV2).
     * @param Q2 Virtuality of the incoming photon (in GeV2).
     * @param E Beam energy in target rest frame (in GeV).
     *  @param phi Angle between leptonic and hadronic plane (in radians, Trento convention).
     */
    void preCompute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, double xB, double t, double Q2,
            double E, double phi);

    double m_xB; ///< Bjorken variable.
    double m_t; ///< Mandelstam variable (square of the 4-momentum transferm in GeV2).
    double m_Q2; ///< Virtuality of the incoming photon (in GeV2).
    double m_E; ///< Beam energy in target rest frame (in GeV).
    double m_phi; ///<  Angle between leptonic and hadronic plane (in radians, Trento convention).

    double m_tmin; ///< Minimal value of t.
    double m_tmax; ///< Maximal value of t.
    double m_xBmin; ///< Minimal value of xB.
    double m_y; ///< Lepton energy fraction.
    double m_epsilon; ///< @f$ \epsilon = \frac{2 x_B M}{Q} @f$.

    DVCSConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule; ///< Pointer to the underlying CCF module.

    DVCSConvolCoeffFunctionKinematic m_lastCCFKinematics; ///< Last Compton Form Factor kinematics.
    DVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.

    /**
     * Bethe-Heitler differential cross section.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Electric charge of the beam (in units of positron charge).
     * @param targetPolarization Target polarization. In GV conventions.
     */
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;

    // beamHelicity

    /**
     * Virtual Compton Scattering differential cross section.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Electric charge of the beam (in units of positron charge).
     * @param targetPolarization Target polarization. In GV conventions.
     */
    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;

    /**
     * Interference differential cross section.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Electric charge of the beam (in units of positron charge).
     * @param targetPolarization Target polarization. In GV conventions.
     */
    virtual double CrossSectionInterf(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;

    /**
     * Gives back a previously computed Compton Form Factor.
     * @param gpdType CFF type (e.g. H or E).
     * @return CFF value for the given type.
     */
    std::complex<double> getConvolCoeffFunctionValue(GPDType::Type gpdType);
};

} /* namespace PARTONS */

#endif /* DVCS_PROCESS_MODULE_H */
