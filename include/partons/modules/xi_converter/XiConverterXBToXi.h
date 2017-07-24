#ifndef XI_CONVERTER_XB_TO_XI_H
#define XI_CONVERTER_XB_TO_XI_H

/**
 * @file XiConverterXBToXi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 October 2015
 * @version 1.0
 */

#include <string>

#include "XiConverterModule.h"

/**
 * @class XiConverterXBToXi
 *
 * @brief Evaluation of GPD \f$\xi\f$ variable as function of \f$x_{B}\f$ only.
 *
 * This model evaluates GPD \f$\xi\f$ variable as a function of only \f$x_{B}\f$ in the following way <br>
 * \f$\xi = \frac{x_{B}}{2 - x_{B}}\f$ <br>
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class XiConverterXBToXi: public XiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    XiConverterXBToXi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~XiConverterXBToXi();

    virtual XiConverterXBToXi* clone() const;

    virtual double compute(double xB, double t, double Q2);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    XiConverterXBToXi(const XiConverterXBToXi &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* XI_CONVERTER_XB_TO_XI_H */
