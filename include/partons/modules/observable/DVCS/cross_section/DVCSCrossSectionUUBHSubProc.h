#ifndef DVCSCROSSSECTIONUUBHSUBPROC_H
#define DVCSCROSSSECTIONUUBHSUBPROC_H

/**
 * @file DVCSCrossSectionUUBHSubProc.h
 * @author Pawel Sznajder (NCBJ)
 * @date November 16, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSCrossSectionUUBHSubProc
 * @brief Unpolarized cross-section for BH subprocess only.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * \frac{\mathrm{d}^4\sigma_{UU, BH}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{1}{2} \left(
 * \frac{\mathrm{d}^4\sigma_{BH}^{\rightarrow -}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right) +
 * \frac{\mathrm{d}^4\sigma_{BH}^{\leftarrow -}}{\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi}\left(x_{B}, t, Q^2, \phi\right)
 * \right) \, ,
 * \f$
 *
 * where
 * \f$\sigma_{BH}^{b_{h} b_{c}}\f$ is a single photon production cross-section (BH subprocess only) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVCSCrossSectionUUBHSubProc: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionUUBHSubProc(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionUUBHSubProc();

    virtual DVCSCrossSectionUUBHSubProc* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionUUBHSubProc(const DVCSCrossSectionUUBHSubProc &other);
};

} /* namespace PARTONS */

#endif /* DVCSCROSSSECTIONUUBHSUBPROC_H */
