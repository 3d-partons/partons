/*
 * TCSCFFStandardNucleiSimple.h
 *
 *  Created on: Mar 11, 2020
 *      Author: partons
 */

#ifndef INCLUDE_PARTONS_MODULES_CONVOL_COEFF_FUNCTION_TCS_TCSCFFSTANDARDNUCLEISIMPLE_H_
#define INCLUDE_PARTONS_MODULES_CONVOL_COEFF_FUNCTION_TCS_TCSCFFSTANDARDNUCLEISIMPLE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../../beans/nuclei/Nuclei.h"
#include "TCSCFFStandard.h"

namespace PARTONS {

class TCSCFFStandardNucleiSimple: public TCSCFFStandard, public Nuclei {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    TCSCFFStandardNucleiSimple(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSCFFStandardNucleiSimple();

    virtual TCSCFFStandardNucleiSimple* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other
     */
    TCSCFFStandardNucleiSimple(const TCSCFFStandardNucleiSimple &other);

private:

    virtual void computeDiagonalGPD();
    virtual double computeSquareChargeAveragedGPD(
               const PartonDistribution &partonDistribution);
};

} /* namespace PARTONS */

#endif /* INCLUDE_PARTONS_MODULES_CONVOL_COEFF_FUNCTION_TCS_TCSCFFSTANDARDNUCLEISIMPLE_H_ */
