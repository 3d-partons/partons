#ifndef DDVCS_SCALES_MODULE_H
#define DDVCS_SCALES_MODULE_H

/**
 * @file DDVCSScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/channel/ChannelType.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../ScalesModule.h"

namespace PARTONS {
class Scales;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DDVCSScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales (DDVCS case).
 */
class DDVCSScalesModule: public ScalesModule<DDVCSObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string DDVCS_SCALES_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSScalesModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSScalesModule(const DDVCSScalesModule &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSScalesModule();

    virtual DDVCSScalesModule* clone() const = 0;

    virtual Scales compute(const DDVCSObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* DDVCS_SCALES_MODULE_H */
