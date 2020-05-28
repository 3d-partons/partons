#ifndef DVMP_PROCESS_MODULE_H
#define DVMP_PROCESS_MODULE_H

/**
 * @file DVMPProcessModule.h
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
#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/MesonPolarization.h"
#include "../../../beans/MesonType.h"
#include "../../../beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../beans/observable/DVMP/DVMPObservableResult.h"
#include "../../../utils/type/PhysicalType.h"
#include "../ProcessModule.h"

namespace PARTONS {
class DVMPConvolCoeffFunctionModule;
class DVMPScalesModule;
class DVMPXiConverterModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPProcessModule
 *
 * @brief Abstract class for computing the *differential* cross section of
 * Deeply Virtual Meson Production (DVMP).
 *
 * The cross-section is five-fold differential with respect to the variables: @f$ x_B @f$, @f$ Q^2 @f$, @f$ t @f$, the two angles and it is defined for a specif meson.
 */
class DVMPProcessModule: public ProcessModule<DVMPObservableKinematic,
        DVMPObservableResult> {

public:

    static const std::string DVMP_PROCESS_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~DVMPProcessModule();

    virtual DVMPProcessModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    /**
     * Computes the differential cross-section. Must be implemented in the child class.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Charge of the beam (in units of positron charge).
     * @param targetPolarization Polarization of the target. In GV conventions.
     * @param mesonPolarization Meson polarization.
     * @param kinematic Kinematics.
     * @param processType Subprocess type.
     * @return Result.
     */
    DVMPObservableResult compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            MesonPolarization::Type mesonPolarization,
            const DVMPObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

    /**
     * Reset previous kinematics.
     */
    virtual void resetPreviousKinematic();

    /**
     * Check if this kinematics is different than the previous one.
     */
    bool isPreviousCCFKinematicDifferent(
            const DVMPConvolCoeffFunctionKinematic& kinematic) const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get scale module.
     */
    DVMPScalesModule* getScaleModule() const;

    /**
     * Set scale module.
     */
    void setScaleModule(DVMPScalesModule* pScaleModule);

    /**
     * Get xi converter module.
     */
    DVMPXiConverterModule* getXiConverterModule() const;

    /**
     * Set xi converted module.
     */
    void setXiConverterModule(DVMPXiConverterModule* pXiConverterModule);

    /**
     * Get CCF module;
     */
    DVMPConvolCoeffFunctionModule* getConvolCoeffFunctionModule() const;

    /**
     * Set CCF module
     */
    void setConvolCoeffFunctionModule(
            DVMPConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

    // ##### IMPLEMENTATION MEMBERS #####

    /**
     * Differential cross section.
     */
    virtual PhysicalType<double> CrossSection();

protected:

    /**
     * Default constructor.
     */
    DVMPProcessModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPProcessModule(const DVMPProcessModule& other);

    virtual void setKinematics(const DVMPObservableKinematic& kinematic);
    virtual void setExperimentalConditions(double beamHelicity,
            double beamCharge, NumA::Vector3D targetPolarization,
            MesonPolarization::Type mesonPolarization);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    double m_xB; ///< Bjorken variable.
    double m_t; ///< Mandelstam variable (square of the 4-momentum transferm in GeV2).
    double m_Q2; ///< Virtuality of the incoming photon (in GeV2).
    double m_E; ///< Beam energy in target rest frame (in GeV).
    double m_phi; ///<  Angle between leptonic and hadronic plane (in radians, Trento convention).
    MesonType::Type m_mesonType; ///< Meson type.

    double m_beamHelicity; ///< Beam helicity.
    double m_beamCharge; ///< Beam charge.
    NumA::Vector3D m_targetPolarization; ///< Target polarization.
    MesonPolarization::Type m_mesonPolarization; ///< Meson polarization.

    double m_tmin; ///< Minimal value of t.
    double m_tmax; ///< Maximal value of t.
    double m_xBmin; ///< Minimal value of xB.
    double m_y; ///< Lepton energy fraction.
    double m_epsilon; ///< @f$ \epsilon = \frac{2 x_B M}{Q} @f$.

    DVMPScalesModule* m_pScaleModule; ///< Pointer to the underlying scale module.
    DVMPXiConverterModule* m_pXiConverterModule; ///< Pointer to the underlying xi converter module.
    DVMPConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule; ///< Pointer to the underlying CCF module.

    DVMPConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.
    DVMPConvolCoeffFunctionKinematic m_lastCCFKinematics; ///< Last Compton Form Factor kinematics.

    /**
     * Compute CCF for a given kinematics.
     * @param kinematic Kinematics to be computed.
     * @param mesonPolarization Meson polarization.
     * @param gpdType List of GPD types to be computed.
     */
    void computeConvolCoeffFunction(const DVMPObservableKinematic& kinematic,
            MesonPolarization::Type mesonPolarization,
            const List<GPDType> & gpdType = List<GPDType>());

    /**
     * Gives back a previously computed Compton Form Factor.
     * @param gpdType CFF type (e.g. H or E).
     * @return CFF value for the given type.
     */
    std::complex<double> getConvolCoeffFunctionValue(GPDType::Type gpdType);
};

} /* namespace PARTONS */

#endif /* DVMP_PROCESS_MODULE_H */
