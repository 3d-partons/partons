#ifndef GAM2_PROCESS_MODULE_H
#define GAM2_PROCESS_MODULE_H

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/GAM2/GAM2ObservableKinematic.h"
#include "../../../beans/observable/GAM2/GAM2ObservableResult.h"
#include "../../../beans/PolarizationType.h"
#include "../../../utils/type/PhysicalType.h"
#include "../ProcessModule.h"

namespace PARTONS {
class GAM2ConvolCoeffFunctionModule;
class GAM2ScalesModule;
class GAM2XiConverterModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class GAM2ProcessModule
 *
 * @brief Abstract class for computing the *differential* cross section of
 * the XXX.
 *
 * The cross-section is five-fold differential with respect to the variables: @f$ x_B @f$, @f$ Q^2 @f$, @f$ t @f$ and the two angles. XXX
 */
class GAM2ProcessModule: public ProcessModule<GAM2ObservableKinematic,
        GAM2ObservableResult> {

public:

    static const std::string GAM2_PROCESS_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~GAM2ProcessModule();

    virtual GAM2ProcessModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    /**
     * Computes the differential cross-section. Must be implemented in the child class.
     */
    virtual GAM2ObservableResult compute(PolarizationType::Type polG0,
            PolarizationType::Type polG1, PolarizationType::Type polG2,
            NumA::Vector3D targetPolarization,
            const GAM2ObservableKinematic& kinematic,
            const List<GPDType>& gpdType = List<GPDType>());

    /**
     * Reset previous kinematics.
     */
    virtual void resetPreviousKinematic();

    /**
     * Check if this kinematics is different than the previous one.
     */
    bool isPreviousCCFKinematicDifferent(
            const GAM2ConvolCoeffFunctionKinematic& kinematic) const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get scale module.
     */
    GAM2ScalesModule* getScaleModule() const;

    /**
     * Set scale module.
     */
    void setScaleModule(GAM2ScalesModule* pScaleModule);

    /**
     * Get xi converter module.
     */
    GAM2XiConverterModule* getXiConverterModule() const;

    /**
     * Set xi converted module.
     */
    void setXiConverterModule(GAM2XiConverterModule* pXiConverterModule);

    /**
     * Get CCF module;
     */
    GAM2ConvolCoeffFunctionModule* getConvolCoeffFunctionModule() const;

    /**
     * Set CCF module
     */
    void setConvolCoeffFunctionModule(
            GAM2ConvolCoeffFunctionModule* pConvolCoeffFunctionModule);

    // ##### IMPLEMENTATION MEMBERS #####

    /**
     * Differential cross section.
     */
    virtual PhysicalType<double> CrossSection();

protected:

    /**
     * Default constructor.
     */
    GAM2ProcessModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2ProcessModule(const GAM2ProcessModule& other);

    virtual void setKinematics(const GAM2ObservableKinematic& kinematic);
    virtual void setExperimentalConditions(PolarizationType::Type polG0,
            PolarizationType::Type polG1, PolarizationType::Type polG2,
            NumA::Vector3D targetPolarization);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    double m_t; ///< Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
    double m_uPrim; ///<F our-momentum transfer squared to photon (in \f$GeV^{2}\f$).
    double m_Mgg2; ///< Invariant mass of two photons (in \f$GeV^{2}\f$).
    double m_E; ///<  Beam energy (in GeV).
    double m_phi; ///< Linear polarization angle.

    double m_tmin; ///< t min;
    double m_xi; ///< xi;

    PolarizationType::Type m_polG0; ///< Polarization of incoming photon.
    PolarizationType::Type m_polG1; ///< Polarization of first outgoing photon.
    PolarizationType::Type m_polG2; ///< Polarization of second outgoing photon.
    NumA::Vector3D m_targetPolarization; ///< Target polarization.

    GAM2ScalesModule* m_pScaleModule; ///< Pointer to the underlying scale module.
    GAM2XiConverterModule* m_pXiConverterModule; ///< Pointer to the underlying xi converter module.
    GAM2ConvolCoeffFunctionModule* m_pConvolCoeffFunctionModule; ///< Pointer to the underlying CCF module.

    GAM2ConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.
    GAM2ConvolCoeffFunctionKinematic m_lastCCFKinematics; ///< Last Compton Form Factor kinematics.

    /**
     * Compute CCF for a given kinematics.
     * @param kinematic Kinematics to be computed.
     * @param gpdType List of GPD types to be computed.
     */
    void computeConvolCoeffFunction(const GAM2ObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());

    /**
     * Gives back a previously computed Compton Form Factor.
     * @param gpdType CFF type (e.g. H or E).
     * @return CFF value for the given type.
     */
    std::complex<double> getConvolCoeffFunctionValue(GPDType::Type gpdType);
};

} /* namespace PARTONS */

#endif /* GAM2_PROCESS_MODULE_H */
