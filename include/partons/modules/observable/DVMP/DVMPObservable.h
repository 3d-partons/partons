#ifndef DVMP_OBSERVABLE_H
#define DVMP_OBSERVABLE_H

/**
 * @file DVMPObservable.h
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
#include "../../../beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../beans/observable/DVMP/DVMPObservableResult.h"
#include "../../../utils/type/PhysicalType.h"
#include "../Observable.h"

namespace PARTONS {
class DVMPProcessModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPObservable
 *
 * @brief Abstract class that provides a skeleton to implement a DVMPObservable module.
 */
class DVMPObservable: public Observable<DVMPObservableKinematic,
        DVMPObservableResult> {

public:

    static const std::string DVMP_OBSERVABLE_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor
     */
    virtual ~DVMPObservable();

    virtual Observable* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);
    virtual DVMPObservableResult compute(
            const DVMPObservableKinematic& kinematic,
            const List<GPDType> & gpdType = List<GPDType>());
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;
    virtual std::vector<double> test();

    // ##### GETTERS & SETTERS #####

    /**
     * Get process module.
     */
    DVMPProcessModule* getProcessModule() const;

    /**
     * Set process module.
     */
    void setProcessModule(DVMPProcessModule* pProcessModule);

protected:

    /**
     * Default constructor.
     */
    DVMPObservable(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPObservable(const DVMPObservable& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
            const List<GPDType>& gpdType) = 0;

    DVMPProcessModule* m_pProcessModule; ///< Pointer to the underlying process module.
};

} /* namespace PARTONS */

#endif /* DVMP_OBSERVABLE_H */
