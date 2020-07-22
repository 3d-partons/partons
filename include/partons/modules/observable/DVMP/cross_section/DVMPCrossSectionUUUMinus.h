#ifndef DVMPCROSSSECTIONUUUMINUS_H
#define DVMPCROSSSECTIONUUUMINUS_H

/**
 * @file DVMPCrossSectionUUUMinus.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVMPObservable.h"

namespace PARTONS {

/**
 * @class DVMPCrossSectionUUUMinus
 * @brief Unpolarized cross-section for negative beam charge.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVMPCrossSectionUUUMinus: public DVMPObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPCrossSectionUUUMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPCrossSectionUUUMinus();

    virtual DVMPCrossSectionUUUMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPCrossSectionUUUMinus(const DVMPCrossSectionUUUMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVMPCROSSSECTIONUUUMINUS_H */
