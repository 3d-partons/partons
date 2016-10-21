/*
 * GPDBorderFunctionKinematic.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef GPDBORDERFUNCTIONKINEMATIC_H_
#define GPDBORDERFUNCTIONKINEMATIC_H_

#include <ElementaryUtils/parameters/GenericType.h>

#include "GPDKinematic.h"

/**
 * GPD border function kinematics
 */
class GPDBorderFunctionKinematic: public GPDKinematic {

public:

    /**
     * Default constructor
     */
    GPDBorderFunctionKinematic();

    /**
     * Copy constructor
     * @param other Object to be copied
     */
    GPDBorderFunctionKinematic(const GPDBorderFunctionKinematic &other);

    /**
     * Constructor
     * @param xi Skewness
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param MuF2 Square of factorization scale (in GeV)
     * @param MuR2 Square of renormalization scale (in GeV)
     */
    GPDBorderFunctionKinematic(double xi, double t, double MuF2, double MuR2);

    /**
     * Constructor
     * @param xi Skewness
     * @param t Mandelstam variable, momentum transfer on the hadron target (in GeV^2)
     * @param MuF2 Square of factorization scale (in GeV)
     * @param MuR2 Square of renormalization scale (in GeV)
     */
    GPDBorderFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t, const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Constructor
     * @param parameters Parameters to be set
     */
    GPDBorderFunctionKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Destructor
     */
    virtual ~GPDBorderFunctionKinematic();

    virtual std::string toString() const;

protected:

    virtual void updateHashSum() const;
};

#endif /* GPDBORDERFUNCTIONKINEMATIC_H_ */
