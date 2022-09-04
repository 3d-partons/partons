#ifndef GAM2_XI_CONVERTER_MODULE_H
#define GAM2_XI_CONVERTER_MODULE_H

/**
 * @file GAM2XiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/observable/GAM2/GAM2ObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "../XiConverterModule.h"

namespace PARTONS {

/**
 * @class GAM2XiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD variable xi (GAM2 case).
 */
class GAM2XiConverterModule: public XiConverterModule<GAM2ObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string GAM2_XI_CONVERTER_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    GAM2XiConverterModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2XiConverterModule(const GAM2XiConverterModule &other);

    /**
     * Destructor.
     */
    virtual ~GAM2XiConverterModule();

    virtual GAM2XiConverterModule* clone() const = 0;

    virtual PhysicalType<double> compute(
            const GAM2ObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* GAM2_XI_CONVERTER_MODULE_H */
