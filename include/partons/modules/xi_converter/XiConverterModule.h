#ifndef XI_CONVERTER_MODULE_H
#define XI_CONVERTER_MODULE_H

/**
 * @file XiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../beans/channel/ChannelType.h"
#include "../../ModuleObject.h"
#include "../../utils/type/PhysicalType.h"

namespace PARTONS {

/**
 * @class XiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD variable xi.
 */
template<typename KinematicType>
class XiConverterModule: public ModuleObject {

public:

    /**
     * Destructor.
     */
    virtual ~XiConverterModule() {
    }

protected:

    /**
     * Constructor.
     * @param className Name of class.
     */
    XiConverterModule(const std::string &className,
            ChannelType::Type channelType) :
            ModuleObject(className, channelType) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    XiConverterModule(const XiConverterModule &other) :
            ModuleObject(other) {
    }

    virtual XiConverterModule* clone() const = 0;

    virtual void initModule() {
    }

    virtual void isModuleWellConfigured() {
    }

    /**
     * Evaluate scales for a given kinematics.
     */
    virtual PhysicalType<double> compute(const KinematicType& kinematic) = 0;
};

} /* namespace PARTONS */

#endif /* XI_CONVERTER_MODULE_H */
