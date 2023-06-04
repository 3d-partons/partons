#ifndef DVCS_XI_CONVERTER_XB_TO_XI_BMP14_H
#define DVCS_XI_CONVERTER_XB_TO_XI_BMP14_H

/**
 * @file DVCSXiConverterXBToXiBMP14.h
 * @author Victor MARTINEZ-FERNANDEZ (NCBJ / Warsaw)
 * @date June 2023
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DVCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class DVCSXiConverterXBToXiBMP14
 *
 * @brief Evaluation of GPD variable xi from \f$x_{B}\f$ for DVCS.
 *
 * \f$\xi = x_{B}(1 + t/Q^2)/(2 - x_{B}(1 - t/Q^2))\f$
 */
class DVCSXiConverterXBToXiBMP14: public DVCSXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSXiConverterXBToXiBMP14(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSXiConverterXBToXiBMP14(const DVCSXiConverterXBToXiBMP14 &other);

    /**
     * Destructor.
     */
    virtual ~DVCSXiConverterXBToXiBMP14();

    virtual DVCSXiConverterXBToXiBMP14* clone() const;
    virtual PhysicalType<double> compute(
            const DVCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DVCS_XI_CONVERTER_XB_TO_XI_BMP14_H */
