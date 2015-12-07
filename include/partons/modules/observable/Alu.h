#ifndef ALU_H
#define ALU_H

/**
 * @file Alu.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 28 November 2014
 * @version 1.0
 *
 * @class Alu
 *
 * @brief
 */

#include <string>

#include "Observable.h"

class ProcessModule;

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

    virtual double compute(ProcessModule* pDVCSModule, double phi);

protected:
    /**
     * Copy constructor
     */
    Alu(const Alu &other);

private:
};

#endif /* ALU_H */
