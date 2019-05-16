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
#include "../../../beans/observable/DVCS/DVCSObservableResult.h"
#include "../../../beans/process/VCSSubProcessType.h"
#include "../../../utils/type/PhysicalType.h"
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
        DVCSObservableResult> {

public:

    static const std::string DVCS_PROCESS_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

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
    virtual DVCSObservableResult compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    /**
     * Computes the differential cross-section. Must be implemented in the child class.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Charge of the beam (in units of positron charge).
     * @param targetPolarization Polarization of the target. In GV conventions.
     * @param kinematic Kinematics.
     * @param processType Subprocess type.
     * @return Result.
     */
    DVCSObservableResult compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType, VCSSubProcessType::Type processType);

    /**
     * Reset previous kinematics.
     */
    virtual void resetPreviousKinematic();

    /**
     * Check if this kinematics is different than the previous one.
     */
    bool isPreviousCCFKinematicDifferent(
            const DVCSConvolCoeffFunctionKinematic& kinematic) const;

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

    // ##### IMPLEMENTATION MEMBERS #####

    /**
     * Bethe-Heitler differential cross section.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Electric charge of the beam (in units of positron charge).
     * @param targetPolarization Target polarization. In GV conventions.
     */
    virtual PhysicalType<double> CrossSectionBH(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization);

    /**
     * Virtual Compton Scattering differential cross section.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Electric charge of the beam (in units of positron charge).
     * @param targetPolarization Target polarization. In GV conventions.
     */
    virtual PhysicalType<double> CrossSectionVCS(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization);

    /**
     * Interference differential cross section.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Electric charge of the beam (in units of positron charge).
     * @param targetPolarization Target polarization. In GV conventions.
     */
    virtual PhysicalType<double> CrossSectionInterf(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization);

protected:

    /**
     * Default constructor.
     */
    DVCSProcessModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSProcessModule(const DVCSProcessModule& other);

    virtual void setKinematics(const DVCSObservableKinematic& kinematic);
    virtual void setExperimentalConditions(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization);
    virtual void initModule();
    virtual void isModuleWellConfigured();

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

    DVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.
    DVCSConvolCoeffFunctionKinematic m_lastCCFKinematics; ///< Last Compton Form Factor kinematics.

    /**
     * Compute CCF for a given kinematics.
     * @param kinematic Kinematics to be computed.
     * @param gpdType List of GPD types to be computed.
     */
    void computeConvolCoeffFunction(const DVCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());

    /**
     * Gives back a previously computed Compton Form Factor.
     * @param gpdType CFF type (e.g. H or E).
     * @return CFF value for the given type.
     */
    std::complex<double> getConvolCoeffFunctionValue(GPDType::Type gpdType);
};

} /* namespace PARTONS */

#endif /* DVCS_PROCESS_MODULE_H */
