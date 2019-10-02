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
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../beans/observable/TCS/TCSObservableResult.h"
#include "../../../utils/type/PhysicalType.h"
#include "../Observable.h"

namespace PARTONS {
class TCSProcessModule;
} /* namespace PARTONS */

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
    virtual TCSObservableResult compute(const TCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get process module.
     */
    TCSProcessModule* getProcessModule() const;

    /**
     * Set process module.
     */
    void setProcessModule(TCSProcessModule* pProcessModule);

protected:

    /**
     * Default constructor.
     */
    TCSObservable(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSObservable(const TCSObservable& other);

    virtual void setKinematics(const TCSObservableKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();
    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType) = 0;

    /**
     * Serialize kinematics and list of GPD types to std::vector<double>.
     */
    std::vector<double> serializeKinematicsAndGPDTypesToVector(
            const TCSObservableKinematic& kin, const List<GPDType>& list) const;

    /**
     * Unserialize kinematics and list of GPD types from std::vector<double>.
     */
    void unserializeKinematicsAndGPDTypesFromVector(
            const std::vector<double>& vec, TCSObservableKinematic& kin,
            List<GPDType>& list) const;

    TCSProcessModule* m_pProcessModule; ///< Pointer to the underlying process module.
};

} /* namespace PARTONS */

#endif /* TCS_OBSERVABLE_H */
