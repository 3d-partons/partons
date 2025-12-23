#ifndef JET_SCALES_MODULE_H
#define JET_SCALES_MODULE_H

/**
 * @file JETScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/channel/ChannelType.h"
#include "../../../beans/observable/JET/JETObservableKinematic.h"
#include "../ScalesModule.h"

namespace PARTONS {
class Scales;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class JETScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales (JET case).
 */
class JETScalesModule: public ScalesModule<JETObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string JET_SCALES_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    JETScalesModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETScalesModule(const JETScalesModule &other);

    /**
     * Destructor.
     */
    virtual ~JETScalesModule();

    virtual JETScalesModule* clone() const = 0;

    virtual Scales compute(const JETObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* JET_SCALES_MODULE_H */
