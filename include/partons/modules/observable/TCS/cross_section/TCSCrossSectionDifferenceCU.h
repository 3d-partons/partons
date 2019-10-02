#ifndef TCSCROSSSECTIONDIFFERENCECU_H
#define TCSCROSSSECTIONDIFFERENCECU_H

/**
 * @file TCSCrossSectionDifferenceCU.h
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
 * @class TCSCrossSectionDifferenceCU
 * @brief Difference of cross-sections obtained with two states of incoming photon circular polarization.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * \Delta\frac{\mathrm{d}^4\sigma_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right) =
 * \frac{1}{2}\left(
 * \frac{\mathrm{d}^4\sigma^{+}_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right) -
 * \frac{\mathrm{d}^4\sigma^{-}_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right)\, .
 * \right)
 * \f$
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class TCSCrossSectionDifferenceCU: public TCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSCrossSectionDifferenceCU(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSCrossSectionDifferenceCU();

    virtual TCSCrossSectionDifferenceCU* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSCrossSectionDifferenceCU(const TCSCrossSectionDifferenceCU &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* TCSCROSSSECTIONDIFFERENCECU_H */
