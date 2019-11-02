#ifndef TCS_PROCESS_BDPGK19_H
#define TCS_PROCESS_BDPGK19_H

/**
 * @file TCSProcessBDPGW19.h
 * @author Oskar Grocholski
 * @date 14 September 2018
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "TCSProcessBDP01.h"

namespace PARTONS {

/**
 * @class TCSProcessBDPGW19
 *
 * Short description.
 *
 * Long description.
 * Source paper: Timelike Compton scattering: exclusive photoproduction of lepton pairs
 * arXiv:hep-ph/0110062v1
 */
class TCSProcessBDPGW19: public TCSProcessBDP01 {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className name of child class.
     */
    TCSProcessBDPGW19(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSProcessBDPGW19();

    virtual TCSProcessBDPGW19* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSProcessBDPGW19(const TCSProcessBDPGW19& other);

    virtual PhysicalType<double> CrossSectionBH();
    virtual PhysicalType<double> CrossSectionVCS();
    virtual PhysicalType<double> CrossSectionInterf();
};

} /* namespace PARTONS */

#endif /* TCS_PROCESS_BDPGK19_H */
