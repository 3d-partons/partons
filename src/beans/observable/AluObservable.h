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

#include <string>

#include "Observable.h"

class AluObservable: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AluObservable(const std::string &className);
    virtual ~AluObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluObservable* clone() const;

    virtual double compute(ObservableModule* pDVCSModule, double phi);

protected:
    /**
     * Copy constructor
     */
    AluObservable(const AluObservable &other);

private:
};

#endif /* ALU_OBSERVABLE_H */
