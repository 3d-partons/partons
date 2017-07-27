#ifndef DVCSCROSSSECTION_H_
#define DVCSCROSSSECTION_H_

/**
 * @file DVCSCrossSection.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date October 15, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSCrossSection
 *
 * @brief Evaluate cross-section with parameters set via automatization mechanism.
 *
 * This module can be used to evaluate a given cross-section with beam charge, beam helicity and target polarization set via the automatization mechanism.
 *
 * Definition:<br>
 *
 * \f$
 * d^5\sigma_{t_{h}}^{b_{h} b_{c}}(x_{B}, t, Q^2, \phi, \phi_{S})
 * \f$
 *
 * where:
 * \f$\sigma_{t_{h}}^{b_{h} b_{c}}\f$ is single photon production cross-section (DVCS, BH and Int) for target helicity denoted by \f$t_{h}\f$, beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 *
 * Units:<br>
 *
 *\f$1/\mathrm{GeV}^6\f$
 */
class DVCSCrossSection: public Observable {

public:

    static const std::string PARAMETER_NAME_BEAM_HELICITY; ///< Name of parameter to set beam helicity via the automatization mechanism.
    static const std::string PARAMETER_NAME_BEAM_CHARGE; ///< Name of parameter to set beam charge via the automatization mechanism.
    static const std::string PARAMETER_NAME_TARGET_POLARIZATION; ///< Name of parameter to set target polarization via the automatization mechanism.

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSection(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSection();

    virtual DVCSCrossSection* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSection(const DVCSCrossSection &other);
};

} /* namespace PARTONS */

#endif /* DVCSCROSSSECTION_H_ */
