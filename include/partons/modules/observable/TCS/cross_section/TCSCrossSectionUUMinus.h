#ifndef TCSCROSSSECTIONUUMINUS_H
#define TCSCROSSSECTIONUUMINUS_H

/**
 * @file TCSCrossSectionUUMinus.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../../../beans/observable/TCS/TCSObservableKinematic.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../TCSObservable.h"

namespace PARTONS {

/**
 * @class TCSCrossSectionUUMinus
 * @brief Unpolarized cross-section for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * \frac{\mathrm{d}^4\sigma_{UU}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{1}{2} \left(
 * \frac{\mathrm{d}^4\sigma^{\rightarrow -}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right) +
 * \frac{\mathrm{d}^4\sigma^{\leftarrow -}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right)
 * \right) \, ,
 * \f$
 *
 * where
 * \f$\sigma^{b_{h} b_{c}}\f$ is a single photon production cross-section (TCS, BH and Interference) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class TCSCrossSectionUUMinus: public TCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSCrossSectionUUMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSCrossSectionUUMinus();

    virtual TCSCrossSectionUUMinus* clone() const;
    virtual PhysicalType<double> computePhiTCSObservable(
            const TCSObservableKinematic& kinematic);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSCrossSectionUUMinus(const TCSCrossSectionUUMinus &other);
};

} /* namespace PARTONS */

#endif /* TCSCROSSSECTIONUUMINUS_H */
