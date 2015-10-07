#ifndef AC_OBSERVABLE_H
#define AC_OBSERVABLE_H

/**
 * @file AcObservable.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 25 November 2014
 * @version 1.0
 *
 * @class AcObservable
 *
 * @brief
 */

#include <string>

#include "Observable.h"

class AcObservable: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AcObservable(const std::string &className);
    virtual ~AcObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcObservable* clone() const;

    virtual double compute(ObservableModule* pDVCSModule, double phi);

protected:
    /**
     * Copy constructor
     */
    AcObservable(const AcObservable &other);

private:
};

#endif /* AC_OBSERVABLE_H */
