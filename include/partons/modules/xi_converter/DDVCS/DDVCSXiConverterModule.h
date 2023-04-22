#ifndef DDVCS_XI_CONVERTER_MODULE_H
#define DDVCS_XI_CONVERTER_MODULE_H

/**
 * @file DDVCSXiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "../XiConverterModule.h"

namespace PARTONS {

/**
 * @class DDVCSXiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD variable xi (DDVCS case).
 */
class DDVCSXiConverterModule: public XiConverterModule<DDVCSObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string DDVCS_XI_CONVERTER_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSXiConverterModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSXiConverterModule(const DDVCSXiConverterModule &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSXiConverterModule();

    virtual DDVCSXiConverterModule* clone() const = 0;

    virtual PhysicalType<double> compute(
            const DDVCSObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* DDVCS_XI_CONVERTER_MODULE_H */
