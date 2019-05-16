#ifndef TCS_OBSERVABLE_H
#define TCS_OBSERVABLE_H

/**
 * @file TCSObservable.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 25, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../beans/observable/TCS/TCSObservableResult.h"
#include "../../../beans/observable/ObservableType.h"
#include "../../../utils/type/PhysicalType.h"
#include "../Observable.h"

namespace PARTONS {

/**
 * @class TCSObservable
 *
 * @brief Abstract class that provides a skeleton to implement a TCSObservable module.
 */
class TCSObservable: public Observable<TCSObservableKinematic,
        TCSObservableResult> {

public:

    static const std::string TCS_OBSERVABLE_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor
     */
    virtual ~TCSObservable();

    virtual Observable* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual TCSObservableResult compute(
            const TCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    // ##### GETTERS & SETTERS #####

    ObservableType::Type getObservableType() const;

    /**
     * Get process module.
     */
    TCSProcessModule* getProcessModule() const;

    /**
     * Set process module.
     */
    void setProcessModule(TCSProcessModule* pProcessModule);

    // ##### IMPLEMENTATION MEMBERS #####

    /**
     * Compute phi dependent observable, invoked if m_observableType = ObservableType::PHI.
     */
    virtual PhysicalType<double> computePhiTCSObservable(
            const TCSObservableKinematic& kinematic);

    /**
     * Compute Fourier-like observable, invoked if m_observableType = ObservableType::FOURIER.
     */
    virtual PhysicalType<double> computeFourierTCSObservable(
            const TCSObservableKinematic& kinematic);

    /**
     * Compute other-like type observable, invoked if m_observableType = ObservableType::UNDEFINED.
     */
    virtual PhysicalType<double> computeOtherTCSObservable(
            const TCSObservableKinematic& kinematic);

protected:

    /**
     * Default constructor.
     */
    TCSObservable(const std::string &className,
            ObservableType::Type observableType);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSObservable(const TCSObservable& other);

    virtual void setKinematics(const TCSObservableKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * Observable type. Determines function to be invoked.
     */
    ObservableType::Type m_observableType;

    double m_xB; ///< Bjorken variable.
    double m_t; ///< Mandelstam variable (square of the 4-momentum transferm in GeV2).
    double m_Q2Prim; ///< Virtuality of the incoming photon (in GeV2).
    double m_E; ///< Beam energy in target rest frame (in GeV).
    double m_phi; ///<  Angle between leptonic and hadronic plane (in radians, Trento convention).
    double m_theta; /// < Angle between positively charged lepton and scattered proton in lepton CMS (in degrees).

    TCSProcessModule* m_pProcessModule; ///< Pointer to the underlying process module.
};

} /* namespace PARTONS */

#endif /* TCS_OBSERVABLE_H */
