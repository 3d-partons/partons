#ifndef DVMPALUUMINUS_H
#define DVMPALUUMINUS_H

/**
 * @file DVMPAluuMinus.h
 * @author Pawel Sznajder (NCBJ)
 * @date September 09, 2020
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVMPObservable.h"

namespace PARTONS {

/**
 * @class DVMPAluuMinus
 * @brief \f$A_{LUU}\f$ asymmetry (long. polarized beam, unpolarized target, unpolarized meson) for negative beam charge.
 *
 * Unit: none.
 */
class DVMPAluuMinus: public DVMPObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPAluuMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPAluuMinus();

    virtual DVMPAluuMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPAluuMinus(const DVMPAluuMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVMPALUUMINUS_H */
