#ifndef CROSSSECTIONOBSERVABLE_H_
#define CROSSSECTIONOBSERVABLE_H_

/**
 * @file CrossSectionObservable.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date 15 October 2015
 * @version 1.0
 *
 * @class CrossSectionObservable
 *
 * @brief
 */

#include <string>

#include "Observable.h"

class CrossSectionObservable: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    CrossSectionObservable(const std::string &className);
    virtual ~CrossSectionObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual CrossSectionObservable* clone() const;

    virtual double compute(ProcessModule* pDVCSModule, double phi);

protected:
    /**
     * Copy constructor
     */
    CrossSectionObservable(const CrossSectionObservable &other);

private:
};

#endif /* CROSSSECTIONOBSERVABLE_H_ */
