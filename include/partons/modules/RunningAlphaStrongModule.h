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

#include <map>
#include <string>

#include "../beans/automation/BaseObjectData.h"
#include "../ModuleObject.h"

class RunningAlphaStrongModule: public ModuleObject {
public:
    static const std::string RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME;

    /**
     * Default constructor
     */
    RunningAlphaStrongModule(const std::string &className);

    virtual RunningAlphaStrongModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~RunningAlphaStrongModule();

    double compute(double Mu2);

    unsigned int getNf() const;
    void setNf(unsigned int nf);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:

    /**
     * Copy constructor
     *
     * @param other
     */
    RunningAlphaStrongModule(const RunningAlphaStrongModule &other);

    void preCompute(const double Mu2);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    bool isPreviousKinematicsDifferent(const double MuF2) const;

    virtual double compute() = 0;

    double m_Mu2;
    double m_Mu;            ///< Current renormalization scale (in GeV)
    unsigned int m_nf;      ///< Number of active flavours
    double m_alphaS;
};

#endif /* MODULE_OBJECT_H */
