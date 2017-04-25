#ifndef AC_H
#define AC_H

/**
 * @file Ac.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 25, 2014
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * @class Ac
 * @brief  Beam charge asymmetry: \f$ A_{C}(\phi)=\frac{1}{4d\sigma_{UU}(\phi)} \left[(d\sigma^{\rightarrow +} + d\sigma^{\leftarrow +} ) - ( d\sigma^{\rightarrow -} + d\sigma^{\leftarrow -}  ) \right]\f$
 *
 * definition reference: arXiv:1210.6975v4 [hep-ph] eq. 45
 */
class Ac: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    Ac(const std::string &className);
    virtual ~Ac();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Ac* clone() const;

    virtual double computePhiObservable(double phi);

protected:
    /**
     * Copy constructor
     */
    Ac(const Ac &other);

private:
};

#endif /* AC_H */
