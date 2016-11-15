#ifndef AluDVCS_H
#define AluDVCS_H

/**
 * @file AluDVCS.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * @class AluDVCS
 * DVCS contribution to beam spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (47)
 */
class AluDVCS: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AluDVCS(const std::string &className);
    virtual ~AluDVCS();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluDVCS* clone() const;

    virtual double computePhiObservable(double phi);

protected:
    /**
     * Copy constructor
     */
    AluDVCS(const AluDVCS &other);

private:
};

#endif /* AluDVCS_H */
