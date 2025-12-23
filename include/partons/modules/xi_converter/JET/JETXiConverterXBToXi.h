#ifndef JET_XI_CONVERTER_XB_TO_XI_H
#define JET_XI_CONVERTER_XB_TO_XI_H

/**
 * @file JETXiConverterXBToXi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "JETXiConverterModule.h"

namespace PARTONS {

/**
 * @class JETXiConverterXBToXi
 *
 * @brief Evaluation of GPD variable xi from \f$x_{B}\f$ for JET.
 *
 * \f$\xi = x_{B}/(2 - x_{B})\f$
 */
class JETXiConverterXBToXi: public JETXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    JETXiConverterXBToXi(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETXiConverterXBToXi(const JETXiConverterXBToXi &other);

    /**
     * Destructor.
     */
    virtual ~JETXiConverterXBToXi();

    virtual JETXiConverterXBToXi* clone() const;
    virtual PhysicalType<double> compute(
            const JETObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* JET_XI_CONVERTER_XB_TO_XI_H */
