#ifndef TCS_XI_CONVERTER_EXACT_H
#define TCS_XI_CONVERTER_EXACT_H

/**
 * @file TCSXiConverterExact.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "TCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class TCSXiConverterExact
 *
 * @brief Exact evaluation of GPD variable xi for TCS.
 *
 * \f$-\xi = \frac{t/2-Q^{2}'}{2 s + t - 2 m^{2} - Q^{2}'}\f$
 */
class TCSXiConverterExact: public TCSXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSXiConverterExact(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSXiConverterExact(const TCSXiConverterExact &other);

    /**
     * Destructor.
     */
    virtual ~TCSXiConverterExact();

    virtual TCSXiConverterExact* clone() const;
    virtual PhysicalType<double> compute(
            const TCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* TCS_XI_CONVERTER_EXACT_H */
