#ifndef TCS_XI_CONVERTER_MODULE_H
#define TCS_XI_CONVERTER_MODULE_H

/**
 * @file TCSXiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "../XiConverterModule.h"

namespace PARTONS {

/**
 * @class TCSXiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD variable xi (TCS case).
 */
class TCSXiConverterModule: public XiConverterModule<TCSObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string TCS_XI_CONVERTER_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSXiConverterModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSXiConverterModule(const TCSXiConverterModule &other);

    /**
     * Destructor.
     */
    virtual ~TCSXiConverterModule();

    virtual TCSXiConverterModule* clone() const = 0;

    virtual PhysicalType<double> compute(
            const TCSObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* TCS_XI_CONVERTER_MODULE_H */
