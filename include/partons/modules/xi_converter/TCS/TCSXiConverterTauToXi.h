#ifndef TCS_XI_CONVERTER_TAU_TO_XI_H
#define TCS_XI_CONVERTER_TAU_TO_XI_H

/**
 * @file TCSXiConverterTauToXi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "TCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class TCSXiConverterTauToXi
 *
 * @brief Evaluation of GPD variable xi from \f$\tau\f$ for TCS.
 *
 * \f$\tau = Q'^{2}/(s-M^{2})\f$
 * \f$\xi = \tau/(2 - \tau)\f$
 */
class TCSXiConverterTauToXi: public TCSXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSXiConverterTauToXi(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSXiConverterTauToXi(const TCSXiConverterTauToXi &other);

    /**
     * Destructor.
     */
    virtual ~TCSXiConverterTauToXi();

    virtual TCSXiConverterTauToXi* clone() const;
    virtual PhysicalType<double> compute(
            const TCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* TCS_XI_CONVERTER_TAU_TO_XI_H */
