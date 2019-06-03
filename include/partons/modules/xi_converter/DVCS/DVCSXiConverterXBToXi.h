#ifndef DVCS_XI_CONVERTER_XB_TO_XI_H
#define DVCS_XI_CONVERTER_XB_TO_XI_H

/**
 * @file DVCSXiConverterXBToXi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DVCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class DVCSXiConverterXBToXi
 *
 * @brief Evaluation of GPD variable xi from \f$x_{B}\f$ for DVCS.
 *
 * \f$\xi = x_{B}/(2 - x_{B})\f$
 */
class DVCSXiConverterXBToXi: public DVCSXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSXiConverterXBToXi(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSXiConverterXBToXi(const DVCSXiConverterXBToXi &other);

    /**
     * Destructor.
     */
    virtual ~DVCSXiConverterXBToXi();

    virtual DVCSXiConverterXBToXi* clone() const;
    virtual PhysicalType<double> compute(
            const DVCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DVCS_XI_CONVERTER_XB_TO_XI_H */
