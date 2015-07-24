#ifndef MODULE_OBJECT_H
#define MODULE_OBJECT_H

/**
 * @file ModuleObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 April 2015
 * @version 1.0
 *
 * @class ModuleObject
 *
 * @brief
 */

#include <string>

#include "../ModuleObject.h"

class RunningAlphaStrongModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    RunningAlphaStrongModule(const std::string &className);

    virtual RunningAlphaStrongModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~RunningAlphaStrongModule();

    virtual double compute(double Mu2) = 0;

    unsigned int getNf() const;
    void setNf(unsigned int nf);

protected:

    /**
     * Copy constructor
     *
     * @param other
     */
    RunningAlphaStrongModule(const RunningAlphaStrongModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    double m_Mu;            ///< Current renormalization scale (in GeV)
    unsigned int m_nf;      ///< Number of active flavours
};

#endif /* MODULE_OBJECT_H */
