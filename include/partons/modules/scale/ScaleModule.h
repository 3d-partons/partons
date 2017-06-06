#ifndef SCALE_MODULE_H
#define SCALE_MODULE_H

/**
 * @file ScaleModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../ModuleObject.h"

class Scale;

/**
 * @class ScaleModule
 *
 * @brief Abstract class for modules evaluating factorization and renormalization scales.
 *
 * This class acts as an abstract (mother) class for modules used in the evaluation of factorization and renormalization scales.
 * The usage of its derivatives is illustrated by the following example:
 \code{.cpp}
 //load one of ScaleModule modules with the BaseModuleFactory
 ScaleModule* pScaleModule = Partons::getInstance()->getModuleObjectFactory()->newScaleModule(Q2Multiplier::classId);

 //evaluate for given Q2 and print
 double Q2 = 10.;

 Scale scales = pScaleModule->compute(Q2);

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "scales at Q2 = "<<  Q2 << " (GeV2) are " << scales.toString());
 \endcode
 which gives via Logger:
 \code
 06-06-2017 03:41:59 [INFO] (example::main) scales at Q2 = 10 (GeV2) are m_MuF2 = 10 (GeV2) m_MuR2 = 10 (GeV2)
 \endcode
 */
class ScaleModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string SCALE_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    ScaleModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ScaleModule();

    virtual ScaleModule* clone() const = 0;

    /**
     * Evaluate scales for a given value of \f$Q^2\f$.
     * @param Q2 Virtual-photon virtuality in \f$GeV^2\f$
     * @return Scale object containing evaluated values.
     */
    virtual Scale compute(double Q2) = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ScaleModule(const ScaleModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* SCALE_MODULE_H */
