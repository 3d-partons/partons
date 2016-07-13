#ifndef AUL_H
#define AUL_H

/**
 * @file Aul.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * @class Aul
 *
 * @brief
 */
class Aul: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    Aul(const std::string &className);
    virtual ~Aul();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Aul* clone() const;

    virtual double compute(double phi);

protected:
    /**
     * Copy constructor
     */
    Aul(const Aul &other);

private:
};

#endif /* AUL_H */
