#ifndef DVMPALLUMINUS_H
#define DVMPALLUMINUS_H

/**
 * @file DVMPAlluMinus.h
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
 * @class DVMPAlluMinus
 * @brief \f$A_{LLU}\f$ asymmetry (long. polarised beam, long. polarized target, unpolarized meson) for negative beam charge.
 *
 * Unit: none.
 */
class DVMPAlluMinus: public DVMPObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPAlluMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPAlluMinus();

    virtual DVMPAlluMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPAlluMinus(const DVMPAlluMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVMPALLUMINUS_H */
