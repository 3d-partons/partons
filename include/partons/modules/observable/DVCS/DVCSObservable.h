#ifndef DVCS_OBSERVABLE_H
#define DVCS_OBSERVABLE_H

/**
 * @file DVCSObservable.h
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
#include "../../../beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../beans/observable/DVCS/DVCSObservableResult.h"
#include "../../../beans/observable/ObservableType.h"
#include "../../../utils/type/PhysicalType.h"
#include "../Observable.h"

namespace PARTONS {

/**
 * @class DVCSObservable
 *
 * @brief Abstract class that provides a skeleton to implement a DVCSObservable module.
 */
class DVCSObservable: public Observable<DVCSObservableKinematic,
        DVCSObservableResult> {

public:

    static const std::string DVCS_OBSERVABLE_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor
     */
    virtual ~DVCSObservable();

    virtual Observable* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual DVCSObservableResult compute(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    // ##### GETTERS & SETTERS #####

    ObservableType::Type getObservableType() const;

    /**
     * Get process module.
     */
    DVCSProcessModule* getProcessModule() const;

    /**
     * Set process module.
     */
    void setProcessModule(DVCSProcessModule* pProcessModule);

    // ##### IMPLEMENTATION MEMBERS #####

    /**
     * Compute phi dependent observable, invoked if m_observableType = ObservableType::PHI.
     */
    virtual PhysicalType<double> computePhiDVCSObservable(
            const DVCSObservableKinematic& kinematic);

    /**
     * Compute Fourier-like observable, invoked if m_observableType = ObservableType::FOURIER.
     */
    virtual PhysicalType<double> computeFourierDVCSObservable(
            const DVCSObservableKinematic& kinematic);

    /**
     * Compute other-like type observable, invoked if m_observableType = ObservableType::UNDEFINED.
     */
    virtual PhysicalType<double> computeOtherDVCSObservable(
            const DVCSObservableKinematic& kinematic);

protected:

    /**
     * Default constructor.
     */
    DVCSObservable(const std::string &className,
            ObservableType::Type observableType);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSObservable(const DVCSObservable& other);

    virtual void setKinematics(const DVCSObservableKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * Observable type. Determines function to be invoked.
     */
    ObservableType::Type m_observableType;

    double m_xB; ///< Bjorken variable.
    double m_t; ///< Mandelstam variable (square of the 4-momentum transferm in GeV2).
    double m_Q2; ///< Virtuality of the incoming photon (in GeV2).
    double m_E; ///< Beam energy in target rest frame (in GeV).
    double m_phi; ///<  Angle between leptonic and hadronic plane (in radians, Trento convention).

    DVCSProcessModule* m_pProcessModule; ///< Pointer to the underlying process module.
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_H */
