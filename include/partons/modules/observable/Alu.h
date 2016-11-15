#ifndef ALU_H
#define ALU_H

/**
 * @file Alu.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * @class Alu
 * Beam spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (48)
 */
class Alu: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    Alu(const std::string &className);
    virtual ~Alu();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Alu* clone() const;

    virtual double computePhiObservable(double phi);

protected:
    /**
     * Copy constructor
     */
    Alu(const Alu &other);

private:
};

#endif /* ALU_H */
