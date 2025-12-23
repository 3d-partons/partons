#ifndef JET_PROCESS_MODULE_H
#define JET_PROCESS_MODULE_H

/**
 * @file JETProcessModule.h
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
#include "../../../beans/convol_coeff_function/JET/JETConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/JET/JETConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/JET/JETObservableKinematic.h"
#include "../../../beans/observable/JET/JETObservableResult.h"
#include "../../../utils/type/PhysicalType.h"
#include "../ProcessModule.h"

namespace PARTONS {
class JETConvolCoeffFunctionModule;
class JETScalesModule;
class JETXiConverterModule;
} /* namespace PARTONS */

namespace NumA {
class Vector3D;
} /* namespace NumA */

namespace PARTONS {

/**
 * @class JETProcessModule
 *
 * @brief Abstract class for computing the *differential* cross section of
 * the photon electroproduction process (also called JET; Deeply Virtual Compton Scattering).
 *
 * The cross-section is five-fold differential with respect to the variables: @f$ x_B @f$, @f$ Q^2 @f$, @f$ t @f$ and the two angles.
 */
class JETProcessModule: public ProcessModule<JETObservableKinematic,
        JETObservableResult> {

public:

    static const std::string JET_PROCESS_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~JETProcessModule();

    virtual JETProcessModule* clone() const = 0;
    virtual void run();
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual JETObservableResult compute(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization,
            const JETObservableKinematic& kinematic,
            const List<GPDType>& gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;
    virtual std::vector<double> test();

    /**
     * Differential cross section.
     */
    virtual PhysicalType<double> CrossSection();

    /**
     * Reset previous kinematics.
     */
    virtual void resetPreviousKinematic();

    /**
     * Check if this kinematics is different than the previous one.
     */
    bool isPreviousCCFKinematicDifferent(
            const JETConvolCoeffFunctionKinematic& kinematic) const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get scale module.
     */
    JETScalesModule* getScaleModule() const;

    /**
     * Set scale module.
     */
    void setScaleModule(JETScalesModule* pScaleModule);

    /**
     * Get xi converter module.
     */
    JETXiConverterModule* getXiConverterModule() const;

    /**
     * Set xi converted module.
     */
    void setXiConverterModule(JETXiConverterModule* pXiConverterModule);

    /**
     * Get CCF module;
     */
    JETConvolCoeffFunctionModule* getConvolCoeffFunctionModule() const;

    /**
     * Set CCF module
     */
    void setConvolCoeffFunctionModule(
            JETConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

protected:

    /**
     * Default constructor.
     */
    JETProcessModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETProcessModule(const JETProcessModule& other);

    virtual void setKinematics(const JETObservableKinematic& kinematic);
    virtual void setExperimentalConditions(double beamHelicity,
        double beamCharge, NumA::Vector3D targetPolarization);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    double m_xB; ///< Bjorken variable.
    double m_t; ///< Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
    double m_z; ///< Longitudinal "minus” momentum fraction carried by the parton forming the jet.
    double m_qPerp2; ///< Square of transverse momentum of parton forming the jet (in \f$GeV^{2}\f$).
    double m_Q2; ///< Virtual-photon virtuality (in \f$GeV^{2}\f$).
    double m_E; ///< Beam energy (in GeV).
    double m_phi; ///< Angle between leptonic and hadronic planes (in radians, Trento convention).
    JetType::Type m_jetType; ///< Jet type.

    double m_beamHelicity; ///< Beam helicity.
    double m_beamCharge; ///< Beam charge.
    NumA::Vector3D m_targetPolarization; ///< Target polarization.

    double m_xBmin; ///< Minimal value of xB.
    double m_y; ///< Lepton energy fraction.

    JETScalesModule* m_pScaleModule; ///< Pointer to the underlying scale module.
    JETXiConverterModule* m_pXiConverterModule; ///< Pointer to the underlying xi converter module.
    JETConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule; ///< Pointer to the underlying CCF module.

    std::map<JetCFFType::Type, JETConvolCoeffFunctionResult> m_jetConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.
    JETConvolCoeffFunctionKinematic m_lastCCFKinematics; ///< Last Compton Form Factor kinematics.

    /**
     * Compute CCF for a given kinematics.
     * @param kinematic Kinematics to be computed.
     * @param gpdType List of GPD types to be computed.
     */
    void computeConvolCoeffFunction(const JETObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());
};

} /* namespace PARTONS */

#endif /* JET_PROCESS_MODULE_H */
