#ifndef AluI_H
#define AluI_H

/**
 * @file Alui.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <string>

#include "Observable.h"

/**
 * @class Alui
 *
 * @brief
 */
class Alui: public Observable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    Alui(const std::string &className);
    virtual ~Alui();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual Alui* clone() const;

    virtual double compute(double phi);

protected:
    /**
     * Copy constructor
     */
    Alui(const Alui &other);

private:
};

#endif /* AluI_H */
