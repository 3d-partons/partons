#ifndef DVMP_XI_CONVERTER_ZETA_TO_XI_H
#define DVMP_XI_CONVERTER_ZETA_TO_XI_H

/**
 * @file DVMPXiConverterZetaToXi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DVMPXiConverterModule.h"

namespace PARTONS {

/**
 * @class DVMPXiConverterZetaToXi
 *
 * @brief Evaluation of GPD variable xi from \f$\zeta\f$ for DVMP.
 *
 * \f$\zeta = m^2/W^2\f$
 * \f$\xi = \zeta/(2 - \zeta)\f$
 *
 * from: https://arxiv.org/pdf/hep-ph/0401131v3.pdf
 */
class DVMPXiConverterZetaToXi: public DVMPXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPXiConverterZetaToXi(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPXiConverterZetaToXi(const DVMPXiConverterZetaToXi &other);

    /**
     * Destructor.
     */
    virtual ~DVMPXiConverterZetaToXi();

    virtual DVMPXiConverterZetaToXi* clone() const;
    virtual PhysicalType<double> compute(
            const DVMPObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DVMP_XI_CONVERTER_ZETA_TO_XI_H */
