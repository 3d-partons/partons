#ifndef TCS_XI_CONVERTER_TAU_TO_XI_NUCLEI_SIMPLE_H
#define TCS_XI_CONVERTER_TAU_TO_XI_NUCLEI_SIMPLE_H

/**
 * @file TCSXiConverterTauToXiNucleiSimple.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/nuclei/Nuclei.h"
#include "../../../utils/type/PhysicalType.h"
#include "TCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class TCSXiConverterTauToXiNucleiSimple
 *
 * @brief Evaluation of GPD variable xi from \f$\tau\f$ for TCS.
 *
 * \f$\tau = Q'^{2}/(s-M^{2})\f$
 * \f$\xi = \tau/(2 - \tau)\f$
 */
class TCSXiConverterTauToXiNucleiSimple: public TCSXiConverterModule,
        public Nuclei {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSXiConverterTauToXiNucleiSimple(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSXiConverterTauToXiNucleiSimple(
            const TCSXiConverterTauToXiNucleiSimple &other);

    /**
     * Destructor.
     */
    virtual ~TCSXiConverterTauToXiNucleiSimple();

    virtual TCSXiConverterTauToXiNucleiSimple* clone() const;
    virtual PhysicalType<double> compute(
            const TCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* TCS_XI_CONVERTER_TAU_TO_XI_NUCLEI_SIMPLE_H */
