#ifndef SCALES_MODULE_H
#define SCALES_MODULE_H

/**
 * @file ScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../beans/channel/ChannelType.h"
#include "../../ModuleObject.h"

namespace PARTONS {
class Scales;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class ScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales.
 */
template<typename KinematicType>
class ScalesModule: public ModuleObject {

public:

    /**
     * Destructor.
     */
    virtual ~ScalesModule() {
    }

protected:

    /**
     * Constructor.
     * @param className Name of class.
     */
    ScalesModule(const std::string &className, ChannelType::Type channelType) :
            ModuleObject(className, channelType) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ScalesModule(const ScalesModule &other) :
            ModuleObject(other) {
    }

    virtual ScalesModule* clone() const = 0;

    virtual void initModule() {
    }

    virtual void isModuleWellConfigured() {
    }

    /**
     * Evaluate scales for a given kinematics.
     */
    virtual Scales compute(const KinematicType& kinematic) = 0;
};

} /* namespace PARTONS */

#endif /* SCALES_MODULE_H */
