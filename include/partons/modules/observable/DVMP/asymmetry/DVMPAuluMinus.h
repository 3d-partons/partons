#ifndef DVMPAULUMINUS_H
#define DVMPAULUMINUS_H

/**
 * @file DVMPAuluMinus.h
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
 * @class DVMPAuluMinus
 * @brief \f$A_{ULU}\f$ asymmetry (unpolarized beam, long. polarized target, unpolarized meson) for negative beam charge.
 *
 * Unit: none.
 */
class DVMPAuluMinus: public DVMPObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPAuluMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPAuluMinus();

    virtual DVMPAuluMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPAuluMinus(const DVMPAuluMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVMPAULUMINUS_H */
