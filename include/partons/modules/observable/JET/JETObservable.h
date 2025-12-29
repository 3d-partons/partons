#ifndef JET_OBSERVABLE_H
#define JET_OBSERVABLE_H

/**
 * @file JETObservable.h
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
#include "../../../beans/observable/JET/JETObservableKinematic.h"
#include "../../../beans/observable/JET/JETObservableResult.h"
#include "../../../utils/type/PhysicalType.h"
#include "../Observable.h"

namespace PARTONS {
class JETProcessModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class JETObservable
 *
 * @brief Abstract class that provides a skeleton to implement a JETObservable module.
 */
class JETObservable: public Observable<JETObservableKinematic,
        JETObservableResult> {

public:

    static const std::string JET_OBSERVABLE_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor
     */
    virtual ~JETObservable();

    virtual Observable* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual JETObservableResult compute(
            const JETObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;
    virtual std::vector<double> test();

    // ##### GETTERS & SETTERS #####

    /**
     * Get process module.
     */
    JETProcessModule* getProcessModule() const;

    /**
     * Set process module.
     */
    void setProcessModule(JETProcessModule* pProcessModule);

protected:

    /**
     * Default constructor.
     */
    JETObservable(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETObservable(const JETObservable& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
    virtual PhysicalType<double> computeObservable(
            const JETObservableKinematic& kinematic,
            const List<GPDType>& gpdType) = 0;

    JETProcessModule* m_pProcessModule; ///< Pointer to the underlying process module.
};

} /* namespace PARTONS */

#endif /* JET_OBSERVABLE_H */
