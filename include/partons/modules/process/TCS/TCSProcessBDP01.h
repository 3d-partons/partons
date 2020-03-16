#ifndef TCS_PROCESS_BDP01_H
#define TCS_PROCESS_BDP01_H

/**
 * @file TCSProcessBDP01.h
 * @author Oskar Grocholski
 * @date 14 September 2018
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "TCSProcessModule.h"

namespace PARTONS {

/**
 * @class TCSProcessBDP01
 *
 * Short description.
 *
 * Long description.
 * Source paper: Timelike Compton scattering: exclusive photoproduction of lepton pairs
 * arXiv:hep-ph/0110062v1
 */
class TCSProcessBDP01: public TCSProcessModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className name of child class.
     */
    TCSProcessBDP01(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSProcessBDP01();

    virtual TCSProcessBDP01* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSProcessBDP01(const TCSProcessBDP01& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSectionBH();
    virtual PhysicalType<double> CrossSectionVCS();
    virtual PhysicalType<double> CrossSectionInterf();

    double F1(double t); ///< Form factor F1.
    double F2(double t); ///< Form factor F2.
    double FA(double t); ///< Form factor FA.
    double FB(double t); ///< Form factor FB.
};

} /* namespace PARTONS */

#endif /* TCS_PROCESS_BDP01_H */
