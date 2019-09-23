#ifndef DVCSCROSSSECTIONDIFFERENCELUMINUS_H
#define DVCSCROSSSECTIONDIFFERENCELUMINUS_H

/**
 * @file DVCSCrossSectionDifferenceLUMinus.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVCSObservable.h"

namespace PARTONS {

/**
 * @class DVCSCrossSectionDifferenceLUMinus
 * @brief Unpolarized cross-section difference for longitudinally polarized beam and negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * \frac{\Delta \mathrm{d}^4\sigma_{LU}}
 * {\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{1}{2} \left(
 * \frac{\mathrm{d}^4\sigma^{\rightarrow -}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right) -
 * \frac{\mathrm{d}^4\sigma^{\leftarrow -}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right)
 * \right) \, ,
 * \f$
 *
 * where
 * \f$\sigma^{b_{h} b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Interference) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVCSCrossSectionDifferenceLUMinus: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionDifferenceLUMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionDifferenceLUMinus();

    virtual DVCSCrossSectionDifferenceLUMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionDifferenceLUMinus(
            const DVCSCrossSectionDifferenceLUMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVCSCROSSSECTIONDIFFERENCELUMINUS_H */
