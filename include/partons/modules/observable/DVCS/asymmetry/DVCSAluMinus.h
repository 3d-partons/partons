#ifndef DVCSALUMINUS_H
#define DVCSALUMINUS_H

/**
 * @file DVCSAluMinus.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVCSObservable.h"

namespace PARTONS {

/**
 * @class DVCSAluMinus
 * @brief Longitudinally polarized beam asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LU}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \mathrm{d}^4\sigma^{\rightarrow -}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma^{\leftarrow -}\left(x_{B}, t, Q^2, \phi\right)
 * }{
 * \mathrm{d}^4\sigma^{\rightarrow -}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma^{\leftarrow -}\left(x_{B}, t, Q^2, \phi\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma^{b_{h} b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Int) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAluMinus: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluMinus();

    virtual DVCSAluMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluMinus(const DVCSAluMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVCSALUMINUS_H */
