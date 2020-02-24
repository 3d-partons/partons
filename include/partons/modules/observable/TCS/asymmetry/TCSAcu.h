#ifndef TCSACU_H
#define TCSACU_H

/**
 * @file TCSAcu.h
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
 * @class TCSAcu
 * @brief Asymmetry probing circular beam polarization.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{CU}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right) =
 * \frac{1}{2}
 * \left(
 * \frac{\mathrm{d}^4\sigma^{+}_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right) -
 * \frac{\mathrm{d}^4\sigma^{-}_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right)
 * \right) /
 * \left(
 * \frac{\mathrm{d}^4\sigma^{+}_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right) +
 * \frac{\mathrm{d}^4\sigma^{-}_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right)
 * \right)\, .
 * \f$
 *
 * Unit: none.
 */
class TCSAcu: public TCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSAcu(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSAcu();

    virtual TCSAcu* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSAcu(const TCSAcu &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* TCSACU_H */
