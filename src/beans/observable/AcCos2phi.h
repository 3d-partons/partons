/*
 * AcCos2phi.h
 *
 *  Created on: Sep 28, 2015
 *      Author: debian
 */

#ifndef ACCOS2PHI_H_
#define ACCOS2PHI_H_

#include "FourrierObservable.h"

class AcObservable;

class AcCos2phi: public FourrierObservable {
public:
    AcCos2phi();
    virtual ~AcCos2phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcCos2phi* clone() const;

protected:
    AcObservable* m_pAcObservable;

    virtual double functionToIntegrate(double *x, double *params);
};

#endif /* ACCOS2PHI_H_ */
