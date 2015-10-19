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
    static const std::string PARAMETER_NAME_HELICITY;
    static const std::string PARAMETER_NAME_CHARGE;
    static const std::string PARAMETER_NAME_TARGET;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    CrossSectionObservable(const std::string &className);
    virtual ~CrossSectionObservable();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual CrossSectionObservable* clone() const;

    virtual double compute(ProcessModule* pDVCSModule, double phi);

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    virtual void configure(ParameterList parameters);

protected:
    /**
     * Copy constructor
     */
    CrossSectionObservable(const CrossSectionObservable &other);

private:
};

#endif /* CROSSSECTIONOBSERVABLE_H_ */
