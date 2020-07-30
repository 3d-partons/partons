#ifndef RUNNING_ALPHA_STRONG_MODULE_H
#define RUNNING_ALPHA_STRONG_MODULE_H

/**
 * @file ModuleObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 April 2015
 * @version 1.0
 */

#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../ModuleObject.h"

namespace PARTONS {

/**
 * @class RunningAlphaStrongModule
 *
 * @brief Abstract class for modules evaluating QCD running coupling constant.
 *
 * This class acts as an abstract (mother) class for modules used in the evaluation of the QCD running coupling constant.
 * The usage of its derivatives is illustrated by the following example:
 \code{.cpp}
 //load one of RunningAlphaStrongModule modules with the BaseModuleFactory
 RunningAlphaStrongModule* pRunningAlphaStrongModule = Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(RunningAlphaStrong::classId);

 //evaluate for given scale and print
 double muR2 = 10.;

 double alpha_s = pRunningAlphaStrongModule->compute(muR2);

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "alpha_s at muR2 = " << muR2 << " GeV2 is " << alpha_s);
 \endcode
 which gives via Logger:
 \code
 06-06-2017 02:25:55 [INFO] (example::main) alpha_s at muR2 = 10GeV2 is 0.250786275698706
 \endcode
 */
class RunningAlphaStrongModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    RunningAlphaStrongModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~RunningAlphaStrongModule();

    virtual RunningAlphaStrongModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Evaluate \f$\alpha_{s}\f$ for a given value of renormalization scale squared.
     * @param Mu2 Value of renormalization scale squared.
     * @return Evaluated value of \f$\alpha_{s}\f$.
     */
    double compute(double Mu2);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    unsigned int getNf() const;
    void setNf(unsigned int nf);

protected:

    /**
     * Copy constructor.
     *
     * @param other Object to be copied.
     */
    RunningAlphaStrongModule(const RunningAlphaStrongModule &other);

    /**
     * Do a pre-evaluation steps before the evaluation of \f$\alpha_{s}\f$.
     * @param Mu2 Value of renormalization scale squared.
     */
    void preCompute(const double Mu2);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * Check if previous kinematics is different than the actual one. This allows to avoid a reevaluation of \f$\alpha_{s}\f$ when the renormalization scale is not changed.
     * @param MuF2 Value of renormalization scale squared.
     * @return True if previous kiematics is different, otherwise false.
     */
    bool isPreviousKinematicsDifferent(const double MuF2) const;

    /**
     * Evaluate \f$\alpha_{s}\f$. The value of renormalization scale should be set at this point by RunningAlphaStrongModule::preCompute() function.
     * @return
     */
    virtual double compute() = 0;

    /**
     * Current renormalization scale squared (in \f$GeV^2\f$)
     */
    double m_Mu2;

    /**
     * Current renormalization scale (in \f$GeV\f$)
     */
    double m_Mu;

    /**
     * Number of active quark flavors.
     */
    unsigned int m_nf;

    /**
     * Value of \f$\alpha_{s}\f$ for the current renormalization scale.
     */
    double m_alphaS;
};

} /* namespace PARTONS */

#endif /* RUNNING_ALPHA_STRONG_MODULE_H */
