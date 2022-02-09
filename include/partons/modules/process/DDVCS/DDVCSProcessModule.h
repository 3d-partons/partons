#ifndef DDVCS_PROCESS_MODULE_H
#define DDVCS_PROCESS_MODULE_H

/**
 * @file DDVCSProcessModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 19, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableResult.h"
#include "../../../beans/process/VCSSubProcessType.h"
#include "../../../utils/type/PhysicalType.h"
#include "../ProcessModule.h"

namespace PARTONS {
class DDVCSConvolCoeffFunctionModule;
class DDVCSScalesModule;
class DDVCSXiConverterModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DDVCSProcessModule
 *
 * @brief Abstract class for computing the *differential* cross section of
 * the photon electroproduction process (also called DDVCS; Deeply Virtual Compton Scattering).
 *
 * The cross-section is five-fold differential with respect to the variables: @f$ x_B @f$, @f$ Q^2 @f$, @f$ t @f$ and the two angles.
 */
class DDVCSProcessModule: public ProcessModule<DDVCSObservableKinematic,
        DDVCSObservableResult> {

public:

    static const std::string DDVCS_PROCESS_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~DDVCSProcessModule();

    virtual DDVCSProcessModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual DDVCSObservableResult compute(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization,
            const DDVCSObservableKinematic& kinematic,
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
    DDVCSObservableResult compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            const DDVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType, VCSSubProcessType::Type processType);

    /**
     * Reset previous kinematics.
     */
    virtual void resetPreviousKinematic();

    /**
     * Check if this kinematics is different than the previous one.
     */
    bool isPreviousCCFKinematicDifferent(
            const DDVCSConvolCoeffFunctionKinematic& kinematic) const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get scale module.
     */
    DDVCSScalesModule* getScaleModule() const;

    /**
     * Set scale module.
     */
    void setScaleModule(DDVCSScalesModule* pScaleModule);

    /**
     * Get xi converter module.
     */
    DDVCSXiConverterModule* getXiConverterModule() const;

    /**
     * Set xi converted module.
     */
    void setXiConverterModule(DDVCSXiConverterModule* pXiConverterModule);

    /**
     * Get CCF module;
     */
    DDVCSConvolCoeffFunctionModule* getConvolCoeffFunctionModule() const;

    /**
     * Set CCF module
     */
    void setConvolCoeffFunctionModule(
            DDVCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

    // ##### IMPLEMENTATION MEMBERS #####

    /**
     * Bethe-Heitler differential cross section.
     */
    virtual PhysicalType<double> CrossSectionBH();

    /**
     * Virtual Compton Scattering differential cross section.
     */
    virtual PhysicalType<double> CrossSectionVCS();

    /**
     * Interference differential cross section.
     */
    virtual PhysicalType<double> CrossSectionInterf();

protected:

    /**
     * Default constructor.
     */
    DDVCSProcessModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSProcessModule(const DDVCSProcessModule& other);

    virtual void setKinematics(const DDVCSObservableKinematic& kinematic);
    virtual void setExperimentalConditions(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    double m_xB; ///< Bjorken variable.
    double m_t; ///< Mandelstam variable (square of the 4-momentum transferm in GeV2).
    double m_Q2; ///< Virtuality of the incoming photon (in GeV2).
    double m_Q2Prim; ///< Virtuality of the outgoing photon (in GeV2).
    double m_E; ///< Beam energy in target rest frame (in GeV).
    double m_phi; ///<  Angle between leptonic and hadronic plane (in radians, Trento convention).
    double m_theta; ///< Angle between positively charged lepton and scattered proton in lepton CMS (in degrees).

    double m_beamHelicity; ///< Beam helicity.
    double m_beamCharge; ///< Beam charge.
    NumA::Vector3D m_targetPolarization; ///< Target polarization.

    DDVCSScalesModule* m_pScaleModule; ///< Pointer to the underlying scale module.
    DDVCSXiConverterModule* m_pXiConverterModule; ///< Pointer to the underlying xi converter module.
    DDVCSConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule; ///< Pointer to the underlying CCF module.

    DDVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.
    DDVCSConvolCoeffFunctionKinematic m_lastCCFKinematics; ///< Last Compton Form Factor kinematics.

    /**
     * Compute CCF for a given kinematics.
     * @param kinematic Kinematics to be computed.
     * @param gpdType List of GPD types to be computed.
     */
    void computeConvolCoeffFunction(const DDVCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());

    /**
     * Gives back a previously computed Compton Form Factor.
     * @param gpdType CFF type (e.g. H or E).
     * @return CFF value for the given type.
     */
    std::complex<double> getConvolCoeffFunctionValue(GPDType::Type gpdType);
};

} /* namespace PARTONS */

#endif /* DDVCS_PROCESS_MODULE_H */
