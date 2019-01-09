#ifndef SCALES_MODULE_H
#define SCALES_MODULE_H

/**
 * @file ScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../ModuleObject.h"
#include "../../utils/type/PhysicalType.h"

namespace PARTONS {

class Scales;

/**
 * @class ScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales.
 */
class ScalesModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string SCALES_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    ScalesModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ScalesModule();

    virtual ScalesModule* clone() const = 0;

    /**
     * Evaluate scales for a given value of \f$Q^2\f$.
     * @param Q2 Virtual-photon virtuality in \f$GeV^2\f$
     * @return Scale object containing evaluated values.
     */
    virtual Scales compute(const PhysicalType<double>& Q2) = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ScalesModule(const ScalesModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* SCALES_MODULE_H */
