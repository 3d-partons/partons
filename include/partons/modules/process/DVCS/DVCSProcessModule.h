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
#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/process/DVCSSubProcessType.h"
#include "../ProcessModule.h"

namespace PARTONS {

/**
 * @class DVCSProcessModule
 *
 * @brief Abstract class for computing the *differential* cross section of
 * the photon electroproduction process (also called DVCS ; Deeply Virtual Compton Scattering).
 *
 * The cross-section is five-fold differential with respect to the variables: @f$ x_B @f$, @f$ Q^2 @f$, @f$ t @f$ and the two angles.
 */
class DVCSProcessModule: public ProcessModule {
public:
    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVCSProcessModule(const std::string &className);
    /**
     * Default destructor.
     */
    virtual ~DVCSProcessModule();

    virtual void resolveObjectDependencies();

    virtual void configure(const ElemUtils::Parameters &parameters);

    void computeConvolCoeffFunction(double xB, double t, double Q2, double E,
            const List<GPDType> & gpdType = List<GPDType>());

    // TODO convert double to integer
    double computeCrossSection(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization, double phi);

    virtual void setConvolCoeffFunctionModule(
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule);
            
    /**
     * Get subprocess type.
     */
    DVCSSubProcessType::Type getSubProcessType() const;

    /**
     * Set subprocess type.
     */
    void setSubProcessType(DVCSSubProcessType::Type subProcessType);

protected:
    /**
     * Copy constructor.
     *
     * Use by the factory.
     *
     * @param other
     */
    DVCSProcessModule(const DVCSProcessModule& other);

    virtual void initModule();
    /**
     * Method called when computing the cross-section, i.e.\ when the angles and polarizations are already known.
     * @param beamHelicity Helicity of the beam.
     * @param beamCharge Electric charge of the beam.
     * @param targetPolarization Target polarization. In GV conventions.
     */
    virtual void initModule(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization);
    virtual void isModuleWellConfigured();

    DVCSSubProcessType::Type m_subProcessType; ///< Switch to select specific subprocess or all of them (default).

    double m_phaseSpace; ///< Phase space factor of the cross section.
    double m_tmin; ///< Minimal value of t.
    double m_tmax; ///< Maximal value of t.
    double m_xBmin; ///< Minimal value of xB.
    double m_y; ///< Lepton energy fraction.
    double m_epsilon; ///< @f$ \epsilon = \frac{2 x_B M}{Q} @f$.

    DVCSConvolCoeffFunctionResult m_dvcsConvolCoeffFunctionResult; ///< Stored Compton Form Factor result.

    // Cross sections
    /**
     * Bethe-Heitler differential cross section.
     * @param beamHelicity Helicity of the beam.
     * @param beamCharge Electric charge of the beam.
     * @param targetPolarization Target polarization. In GV conventions.
     * @return
     */
    virtual double CrossSectionBH(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;
    // beamCharge in units of positron charge
    // beamHelicity in units of hbar/2

    /**
     * Virtual Compton Scattering differential cross section.
     * @param beamHelicity Helicity of the beam.
     * @param beamCharge Electric charge of the beam.
     * @param targetPolarization Target polarization. In GV conventions.
     * @return
     */
    virtual double CrossSectionVCS(double beamHelicity, double beamCharge,
            NumA::Vector3D targetPolarization) = 0;

    /**
     * Interference differential cross section.
     * @param beamHelicity Helicity of the beam.
     * @param beamCharge Electric charge of the beam.
     * @param targetPolarization Target polarization. In GV conventions.
     * @return
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
