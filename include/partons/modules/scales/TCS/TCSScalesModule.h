#ifndef TCS_SCALES_MODULE_H
#define TCS_SCALES_MODULE_H

/**
 * @file TCSScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/channel/ChannelType.h"
#include "../../../beans/observable/TCS/TCSObservableKinematic.h"
#include "../ScalesModule.h"

namespace PARTONS {
class Scales;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class TCSScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales (TCS case).
 */
class TCSScalesModule: public ScalesModule<TCSObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string TCS_SCALES_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSScalesModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSScalesModule(const TCSScalesModule &other);

    /**
     * Destructor.
     */
    virtual ~TCSScalesModule();

    virtual TCSScalesModule* clone() const = 0;

    virtual Scales compute(const TCSObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* TCS_SCALES_MODULE_H */
