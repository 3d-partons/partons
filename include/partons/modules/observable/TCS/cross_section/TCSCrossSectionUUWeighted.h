#ifndef TCSCROSSSECTIONUUWEIGHTED_H
#define TCSCROSSSECTIONUUWEIGHTED_H

/**
 * @file TCSCrossSectionUUWeighted.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../TCSObservable.h"

namespace PARTONS {

/**
 * @class TCSCrossSectionUUWeighted
 * @brief Weighted unpolarized cross-section.
 *
 * Definition:<br>
 *
 * see Eq. (43) of Eur. Phys. J. C23 (2002) 675
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class TCSCrossSectionUUWeighted: public TCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSCrossSectionUUWeighted(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSCrossSectionUUWeighted();

    virtual TCSCrossSectionUUWeighted* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSCrossSectionUUWeighted(const TCSCrossSectionUUWeighted &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* TCSCROSSSECTIONUUWEIGHTED_H */
