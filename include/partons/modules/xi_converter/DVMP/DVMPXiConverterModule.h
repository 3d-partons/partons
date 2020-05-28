#ifndef DVMP_XI_CONVERTER_MODULE_H
#define DVMP_XI_CONVERTER_MODULE_H

/**
 * @file DVMPXiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "../XiConverterModule.h"

namespace PARTONS {

/**
 * @class DVMPXiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD variable xi (DVMP case).
 */
class DVMPXiConverterModule: public XiConverterModule<DVMPObservableKinematic> {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string DVMP_XI_CONVERTER_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPXiConverterModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPXiConverterModule(const DVMPXiConverterModule &other);

    /**
     * Destructor.
     */
    virtual ~DVMPXiConverterModule();

    virtual DVMPXiConverterModule* clone() const = 0;

    virtual PhysicalType<double> compute(
            const DVMPObservableKinematic& kinematic) = 0;

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* DVMP_XI_CONVERTER_MODULE_H */
