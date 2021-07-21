#ifndef DVMP_XI_CONVERTER_XB_TO_XI_H
#define DVMP_XI_CONVERTER_XB_TO_XI_H

/**
 * @file DVMPXiConverterXBToXi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DVMPXiConverterModule.h"

namespace PARTONS {

/**
 * @class DVMPXiConverterXBToXi
 *
 * @brief Evaluation of GPD variable xi from \f$x_{B}\f$ for DVMP.
 *
 * \f$\xi = x_{B}/(2 - x_{B})\f$
 */
class DVMPXiConverterXBToXi: public DVMPXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPXiConverterXBToXi(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPXiConverterXBToXi(const DVMPXiConverterXBToXi &other);

    /**
     * Destructor.
     */
    virtual ~DVMPXiConverterXBToXi();

    virtual DVMPXiConverterXBToXi* clone() const;
    virtual PhysicalType<double> compute(
            const DVMPObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DVMP_XI_CONVERTER_XB_TO_XI_H */
