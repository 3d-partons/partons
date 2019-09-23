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
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../beans/observable/DVCS/DVCSObservableResult.h"
#include "../../../utils/type/PhysicalType.h"
#include "../Observable.h"

namespace PARTONS {
class DVCSProcessModule;
} /* namespace PARTONS */

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

    /**
     * Get process module.
     */
    DVCSProcessModule* getProcessModule() const;

    /**
     * Set process module.
     */
    void setProcessModule(DVCSProcessModule* pProcessModule);

protected:

    /**
     * Default constructor.
     */
    DVCSObservable(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSObservable(const DVCSObservable& other);

    virtual void setKinematics(const DVCSObservableKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();
    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType) = 0;

    /**
     * List of GPD types to std::vector<double>.
     */
    std::vector<double> gpdTypesToVector(const List<GPDType>& list) const;

    /**
     * std::vector<double> to list of GPD types.
     */
    List<GPDType> gpdTypesFromVector(const std::vector<double>& vec) const;

    double m_xB; ///< Bjorken variable.
    double m_t; ///< Mandelstam variable (square of the 4-momentum transferm in GeV2).
    double m_Q2; ///< Virtuality of the incoming photon (in GeV2).
    double m_E; ///< Beam energy in target rest frame (in GeV).
    double m_phi; ///<  Angle between leptonic and hadronic plane (in radians, Trento convention).

    DVCSProcessModule* m_pProcessModule; ///< Pointer to the underlying process module.
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_H */
