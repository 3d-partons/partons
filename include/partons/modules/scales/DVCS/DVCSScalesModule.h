#ifndef DVCS_SCALES_MODULE_H
#define DVCS_SCALES_MODULE_H

/**
 * @file DVCSScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/channel/ChannelType.h"
#include "../../../beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../ScalesModule.h"

namespace PARTONS {
class Scales;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVCSScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales (DVCS case).
 */
class DVCSScalesModule: public ScalesModule<DVCSObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string DVCS_SCALES_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSScalesModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSScalesModule(const DVCSScalesModule &other);

    /**
     * Destructor.
     */
    virtual ~DVCSScalesModule();

    virtual DVCSScalesModule* clone() const = 0;

    virtual Scales compute(const DVCSObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* DVCS_SCALES_MODULE_H */
