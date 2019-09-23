#ifndef DVCSAUTINTSINPHIMPHIS_H
#define DVCSAUTINTSINPHIMPHIS_H

/**
 * @file DVCSAutIntSinPhiMPhis.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVCSObservable.h"

namespace PARTONS {

/**
 * @class DVCSAutIntSinPhiMPhis
 * @brief Transverse target asymmetry (interference part).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UT, Int}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \left(\mathrm{d}^4\sigma_{\downarrow}^{+}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma_{\downarrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right) -
 * \left(\mathrm{d}^4\sigma_{\uparrow}^{+}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma_{\uparrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right)
 * }{
 * \left(\mathrm{d}^4\sigma_{\downarrow}^{+}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\downarrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right) +
 * \left(\mathrm{d}^4\sigma_{\uparrow}^{+}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\uparrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma_{t_{h}}^{b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Int) for target helicity denoted by \f$t_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAutIntSinPhiMPhis: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAutIntSinPhiMPhis(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutIntSinPhiMPhis();

    virtual DVCSAutIntSinPhiMPhis* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutIntSinPhiMPhis(const DVCSAutIntSinPhiMPhis &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVCSAUTINTSINPHIMPHIS_H */
