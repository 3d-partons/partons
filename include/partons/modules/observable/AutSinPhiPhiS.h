#ifndef AutSinPhiPhiS_H
#define AutSinPhiPhiS_H

/**
 * @file AutSinPhiPhiS.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * @class AutSinPhiPhiS
 * target longitudianl spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (50)
 */
class AutSinPhiPhiS: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AutSinPhiPhiS(const std::string &className);
    virtual ~AutSinPhiPhiS();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AutSinPhiPhiS* clone() const;

    virtual double computePhiObservable(double phi);

protected:
    /**
     * Copy constructor
     */
    AutSinPhiPhiS(const AutSinPhiPhiS &other);

private:
};

#endif /* AutSinPhiPhiS_H */
