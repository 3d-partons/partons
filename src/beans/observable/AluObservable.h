#ifndef ALU_OBSERVABLE_H
#define ALU_OBSERVABLE_H

/**
 * @file AluObservable.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 28 November 2014
 * @version 1.0
 *
 * @class AluObservable
 *
 * @brief
 */

#include "Observable.h"

class AluObservable: public Observable {
public:
    AluObservable();
    virtual ~AluObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluObservable* clone() const;

    double compute(DVCSModule* pDVCSModule);

private:
};

#endif /* ALU_OBSERVABLE_H */
