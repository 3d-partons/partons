#ifndef GAM2_SCALES_MODULE_H
#define GAM2_SCALES_MODULE_H

/**
 * @file GAM2ScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/channel/ChannelType.h"
#include "../../../beans/observable/GAM2/GAM2ObservableKinematic.h"
#include "../ScalesModule.h"

namespace PARTONS {
class Scales;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class GAM2ScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales (GAM2 case).
 */
class GAM2ScalesModule: public ScalesModule<GAM2ObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string GAM2_SCALES_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    GAM2ScalesModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2ScalesModule(const GAM2ScalesModule &other);

    /**
     * Destructor.
     */
    virtual ~GAM2ScalesModule();

    virtual GAM2ScalesModule* clone() const = 0;

    virtual Scales compute(const GAM2ObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* GAM2_SCALES_MODULE_H */
