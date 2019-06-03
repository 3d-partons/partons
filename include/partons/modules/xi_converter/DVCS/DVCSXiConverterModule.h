#ifndef DVCS_XI_CONVERTER_MODULE_H
#define DVCS_XI_CONVERTER_MODULE_H

/**
 * @file DVCSXiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "../XiConverterModule.h"

namespace PARTONS {

/**
 * @class DVCSXiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD variable xi (DVCS case).
 */
class DVCSXiConverterModule: public XiConverterModule<DVCSObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string DVCS_XI_CONVERTER_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSXiConverterModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSXiConverterModule(const DVCSXiConverterModule &other);

    /**
     * Destructor.
     */
    virtual ~DVCSXiConverterModule();

    virtual DVCSXiConverterModule* clone() const = 0;

    virtual PhysicalType<double> compute(
            const DVCSObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* DVCS_XI_CONVERTER_MODULE_H */
