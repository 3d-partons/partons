#ifndef TCS_CFF_FROM_DVCS_ONLY_NLO_PART_H
#define TCS_CFF_FROM_DVCS_ONLY_NLO_PART_H

/**
 * @file TCSCFFFromDVCSOnlyNLOPart.h
 * @author Oskar Grocholski (UW/NCBJ)
 * @date September 08, 2014
 * @version 1.0
 */

#include <complex>
#include <string>

#include "TCSCFFFromDVCS.h"

namespace PARTONS {

/**
 * @class TCSCFFFromDVCSOnlyNLOPart
 *
 * This module calculates TCS Compton Form Factors using DVCS CFFs (only NLO addition to LO is given)
 *
 * Available CFF types: H, E, Ht, Et.
 */
class TCSCFFFromDVCSOnlyNLOPart: public TCSCFFFromDVCS {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    TCSCFFFromDVCSOnlyNLOPart(const std::string &className);

    virtual TCSCFFFromDVCSOnlyNLOPart* clone() const;

    /**
     * Default destructor.
     */
    virtual ~TCSCFFFromDVCSOnlyNLOPart();

protected:

    /**
     * Copy constructor.
     * @param other
     */
    TCSCFFFromDVCSOnlyNLOPart(const TCSCFFFromDVCSOnlyNLOPart &other);

    virtual std::complex<double> computeUnpolarized();
};

} /* namespace PARTONS */

#endif /* TCS_CFF_FROM_DVCS_ONLY_NLO_PART_H */
