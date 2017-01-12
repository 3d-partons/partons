#ifndef AutSinPhiPhiSOld_H
#define AutSinPhiPhiSOld_H

/**
 * @file AutSinPhiPhiSOld.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * @class AutSinPhiPhiSOld
 * target longitudianl spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (50)
 */
class AutSinPhiPhiSOld: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AutSinPhiPhiSOld(const std::string &className);
    virtual ~AutSinPhiPhiSOld();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AutSinPhiPhiSOld* clone() const;

    virtual double computePhiObservable(double phi);

protected:
    /**
     * Copy constructor
     */
    AutSinPhiPhiSOld(const AutSinPhiPhiSOld &other);

private:
};

#endif /* AutSinPhiPhiSOld_H */
