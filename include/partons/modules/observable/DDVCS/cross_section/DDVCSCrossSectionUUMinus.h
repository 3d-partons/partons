#ifndef DDVCSCROSSSECTIONUUMINUS_H
#define DDVCSCROSSSECTIONUUMINUS_H

/**
 * @file DDVCSCrossSectionUUMinus.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DDVCSObservable.h"

namespace PARTONS {

/**
 * @class DDVCSCrossSectionUUMinus
 * @brief Unpolarized cross-section for negative beam charge.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DDVCSCrossSectionUUMinus: public DDVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSCrossSectionUUMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DDVCSCrossSectionUUMinus();

    virtual DDVCSCrossSectionUUMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSCrossSectionUUMinus(const DDVCSCrossSectionUUMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DDVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DDVCSCROSSSECTIONUUMINUS_H */
