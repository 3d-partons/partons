#ifndef DVMPCROSSSECTIONUUMINUSPI0_H
#define DVMPCROSSSECTIONUUMINUSPI0_H

/**
 * @file DVMPCrossSectionUUMinusPi0.h
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
 * @class DVMPCrossSectionUUMinusPi0
 * @brief Unpolarized cross-section for negative beam charge - pi0.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVMPCrossSectionUUMinusPi0: public DVMPObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPCrossSectionUUMinusPi0(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPCrossSectionUUMinusPi0();

    virtual DVMPCrossSectionUUMinusPi0* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPCrossSectionUUMinusPi0(const DVMPCrossSectionUUMinusPi0 &other);

    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVMPCROSSSECTIONUUMINUSPI0_H */
