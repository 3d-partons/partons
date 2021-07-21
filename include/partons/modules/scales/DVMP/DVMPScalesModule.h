#ifndef DVMP_SCALES_MODULE_H
#define DVMP_SCALES_MODULE_H

/**
 * @file DVMPScalesModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/channel/ChannelType.h"
#include "../../../beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../ScalesModule.h"

namespace PARTONS {
class Scales;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPScalesModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales (DVMP case).
 */
class DVMPScalesModule: public ScalesModule<DVMPObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string DVMP_SCALES_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPScalesModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPScalesModule(const DVMPScalesModule &other);

    /**
     * Destructor.
     */
    virtual ~DVMPScalesModule();

    virtual DVMPScalesModule* clone() const = 0;

    virtual Scales compute(const DVMPObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* DVMP_SCALES_MODULE_H */
