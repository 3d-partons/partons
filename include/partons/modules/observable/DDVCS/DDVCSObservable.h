#ifndef DDVCS_OBSERVABLE_H
#define DDVCS_OBSERVABLE_H

/**
 * @file DDVCSObservable.h
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
#include "../../../beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableResult.h"
#include "../../../utils/type/PhysicalType.h"
#include "../Observable.h"

namespace PARTONS {
class DDVCSProcessModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DDVCSObservable
 *
 * @brief Abstract class that provides a skeleton to implement a DDVCSObservable module.
 */
class DDVCSObservable: public Observable<DDVCSObservableKinematic,
        DDVCSObservableResult> {

public:

    static const std::string DDVCS_OBSERVABLE_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor
     */
    virtual ~DDVCSObservable();

    virtual Observable* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual DDVCSObservableResult compute(
            const DDVCSObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get process module.
     */
    DDVCSProcessModule* getProcessModule() const;

    /**
     * Set process module.
     */
    void setProcessModule(DDVCSProcessModule* pProcessModule);

protected:

    /**
     * Default constructor.
     */
    DDVCSObservable(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSObservable(const DDVCSObservable& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
    virtual PhysicalType<double> computeObservable(
            const DDVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType) = 0;

    DDVCSProcessModule* m_pProcessModule; ///< Pointer to the underlying process module.
};

} /* namespace PARTONS */

#endif /* DDVCS_OBSERVABLE_H */
