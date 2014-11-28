#ifndef AC_OBSERVABLE_H
#define AC_OBSERVABLE_H

/**
 * @file AcObservable.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 25 November 2014
 * @version 1.0
 *
 * @class AcObservable
 */

#include "Observable.h"

class AcObservable: public Observable {
public:
    AcObservable();
    virtual ~AcObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcObservable* clone() const;

    virtual double compute(DVCSModule* pDVCSModule);

private:
};

#endif /* AC_OBSERVABLE_H */
