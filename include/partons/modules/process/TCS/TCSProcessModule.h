#ifndef TCS_PROCESS_MODULE_H
#define TCS_PROCESS_MODULE_H

/**
 * @file TCSProcessModule.h
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
#include "../../../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../beans/observable/TCS/TCSObservableResult.h"
#include "../../../beans/process/VCSSubProcessType.h"
#include "../../../utils/type/PhysicalType.h"
#include "../ProcessModule.h"

namespace PARTONS {
class TCSConvolCoeffFunctionModule;
class TCSScalesModule;
class TCSXiConverterModule;
} /* namespace PARTONS */

namespace NumA {
class Vector3D;
} /* namespace NumA */

namespace PARTONS {

/**
 * @class TCSProcessModule
 *
 * @brief Abstract class for computing the *differential* cross section of
 * the photon electroproduction process (also called TCS; Deeply Virtual Compton Scattering).
 *
 * The cross-section is five-fold differential with respect to the variables: @f$ x_B @f$, @f$ Q^2 @f$, @f$ t @f$ and the two angles.
 */
class TCSProcessModule: public ProcessModule<TCSObservableKinematic,
        TCSObservableResult> {

public:

    static const std::string TCS_PROCESS_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~TCSProcessModule();

    virtual TCSProcessModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual TCSObservableResult compute(double beamPolarization,
            NumA::Vector3D targetPolarization,
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;
    virtual std::vector<double> test();

    /**
     * Computes the differential cross-section. Must be implemented in the child class.
     * @param beamHelicity Helicity of the beam (in units of hbar/2).
     * @param beamCharge Charge of the beam (in units of positron charge).
     * @param targetPolarization Polarization of the target. In GV conventions.
     * @param kinematic Kinematics.
     * @param processType Subprocess type.
     * @return Result.
     */
    TCSObservableResult compute(double beamPolarization,
            NumA::Vector3D targetPolarization,
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType, VCSSubProcessType::Type processType);

    /**
     * Reset previous kinematics.
     */
    virtual void resetPreviousKinematic();

    /**
     * Check if this kinematics is different than the previous one.
     */
    bool isPreviousCCFKinematicDifferent(
            const TCSConvolCoeffFunctionKinematic& kinematic) const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get scale module.
     */
    TCSScalesModule* getScaleModule() const;

    /**
     * Set scale module.
     */
    void setScaleModule(TCSScalesModule* pScaleModule);

    /**
     * Get xi converter module.
     */
    TCSXiConverterModule* getXiConverterModule() const;

    /**
     * Set xi converted module.
     */
    void setXiConverterModule(TCSXiConverterModule* pXiConverterModule);

    /**
     * Get CCF module;
     */
    TCSConvolCoeffFunctionModule* getConvolCoeffFunctionModule() const;

    /**
     * Set CCF module
     */
    void setConvolCoeffFunctionModule(
            TCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

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

    /**
     * Set manually CFFs.
     */
    void setConvolCoeffFunction(const TCSConvolCoeffFunctionKinematic& kin,
            const TCSConvolCoeffFunctionResult& result);

protected:

    /**
     * Default constructor.
     */
    TCSProcessModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSProcessModule(const TCSProcessModule& other);

    virtual void setKinematics(const TCSObservableKinematic& kinematic);
    virtual void setExperimentalConditions(double beamPolarization,
            NumA::Vector3D targetPolarization);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    double m_t; ///< Mandelstam variable (square of the 4-momentum transferm in GeV2).
    double m_Q2Prim; ///< Virtuality of the incoming photon (in GeV2).
    double m_E; ///< Beam energy in target rest frame (in GeV).
    double m_phi; ///<  Angle between leptonic and hadronic plane (in radians, Trento convention).
    double m_theta; ///< Angle between positively charged lepton and scattered proton in lepton CMS (in degrees).

    double m_beamPolarization; ///< Beam polarization.
    NumA::Vector3D m_targetPolarization; ///< Target polarization.

    double m_tmin; ///< Minimal value of t.
    double m_tmax; ///< Maximal value of t.

    TCSScalesModule* m_pScaleModule; ///< Pointer to the underlying scale module.
    TCSXiConverterModule* m_pXiConverterModule; ///< Pointer to the underlying xi converter module.
    TCSConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule; ///< Pointer to the underlying CCF module.

    TCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.
    TCSConvolCoeffFunctionKinematic m_lastCCFKinematics; ///< Last Compton Form Factor kinematics.

    /**
     * Compute CCF for a given kinematics.
     * @param kinematic Kinematics to be computed.
     * @param gpdType List of GPD types to be computed.
     */
    void computeConvolCoeffFunction(const TCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());

    /**
     * Gives back a previously computed Compton Form Factor.
     * @param gpdType CFF type (e.g. H or E).
     * @return CFF value for the given type.
     */
    std::complex<double> getConvolCoeffFunctionValue(GPDType::Type gpdType);
};

} /* namespace PARTONS */

#endif /* TCS_PROCESS_MODULE_H */
