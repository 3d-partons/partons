#ifndef XI_CONVERTER_MODULE
#define XI_CONVERTER_MODULE

/**
 * @file XiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 05, 2015
 * @version 1.0
 */

#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../ModuleObject.h"

namespace PARTONS {

/**
 * @class XiConverterModule
 *
 * @brief Abstract class for modules evaluating GPD \f$\xi\f$ variable.
 *
 * This class acts as an abstract (mother) class for modules used in the evaluation of GPD \f$\xi\f$ variable.
 * The usage of its derivatives is illustrated by the following example:
 \code{.cpp}
 //load one of XiConverterModule modules with the BaseModuleFactory
 XiConverterModule* pXiConverterModule = Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(XBToXi::classId);

 //evaluate for given scale and print
 double xB = 0.1;
 double t = -0.2;
 double Q2 = 10.;

 double xi = pXiConverterModule->compute(xB, t, Q2);

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "xi for xB = " <<  xB << ", t = " << t << " (GeV2), Q2 = " << Q2 << " (GeV2) is " << xi);
 \endcode
 which gives via Logger:
 \code
 06-06-2017 04:05:47 [INFO] (example::main) xi for xB = 0.1, t = -0.2 (GeV2), Q2 = 10 (GeV2) is 0.0526315789473684
 \endcode
 */
class XiConverterModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string XI_CONVERTER_MODULE_CLASS_NAME;

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    XiConverterModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~XiConverterModule();

    virtual XiConverterModule* clone() const = 0;

    /**
     * Evaluate GPD \f$\xi\f$ variable for given values of \f$x_{B}\f$, \f$t\f$ and \f$Q^2\f$.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @return Evaluated value.
     */
    virtual double compute(double xB, double t, double Q2) = 0;

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    XiConverterModule(const XiConverterModule& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();
};

} /* namespace PARTONS */

#endif /* XI_CONVERTER_MODULE */
