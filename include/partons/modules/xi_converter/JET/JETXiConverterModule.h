#ifndef JET_XI_CONVERTER_MODULE_H
#define JET_XI_CONVERTER_MODULE_H

/**
 * @file JETXiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/observable/JET/JETObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "../XiConverterModule.h"

namespace PARTONS {

/**
 * @class JETXiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD variable xi (JET case).
 */
class JETXiConverterModule: public XiConverterModule<JETObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string JET_XI_CONVERTER_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    JETXiConverterModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETXiConverterModule(const JETXiConverterModule &other);

    /**
     * Destructor.
     */
    virtual ~JETXiConverterModule();

    virtual JETXiConverterModule* clone() const = 0;

    virtual PhysicalType<double> compute(
            const JETObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* JET_XI_CONVERTER_MODULE_H */
