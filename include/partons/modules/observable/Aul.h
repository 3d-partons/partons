#ifndef AUL_H
#define AUL_H

/**
 * @file Aul.h
 * @author Luca COLANERI (IPNO ORSAY)
 * @date 5 April 2016
 * @version 1.0
 *
 * @class Aul
 *
 * @brief
 */

#include <string>

#include "Observable.h"

class ProcessModule;

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

    virtual double Num(ProcessModule* pDVCSModule, double phi);
    virtual double Den(ProcessModule* pDVCSModule, double phi);

    virtual double compute(ProcessModule* pDVCSModule, double phi);

protected:
    /**
     * Copy constructor
     */
    Aul(const Aul &other);

private:
};

#endif /* AUL_H */
