#ifndef ALL_H
#define ALL_H

/**
 * @file All.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * double longitudinal target spin asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (51)
 */
class All: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    All(const std::string &className);
    virtual ~All();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual All* clone() const;

    virtual double computePhiObservable(double phi); ///< calculate cross sections with various helicities and beam charge and combine them to obtain the asymmetry

protected:
    /**
     * Copy constructor
     */
    All(const All &other);

private:
};

#endif /* ALL_H */
