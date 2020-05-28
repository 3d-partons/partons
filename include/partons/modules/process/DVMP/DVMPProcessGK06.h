#ifndef DVMP_PROCESS_VGG99_H
#define DVMP_PROCESS_VGG99_H

/**
 * @file DVMPProcessGK06.h
 * @author  Michel Guidal (IPNO)
 * @author Pawel Sznajder (IPNO)
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DVMPProcessModule.h"

namespace PARTONS {

/**
 * @class DVMPProcessGK06
 *
 * VGG process model for DVMP.
 *
 * For the reference see:
 * - Prog. Part. Nucl. Phys. 47, 401 (2001)
 * - Phys. Rev. Lett. 80 5064 (1998).
 * - Phys. Rev. D 60, 094017 (1999).
 * - Phys. Rev. D 72, 054013 (2005).
 *
 * Module based on the original code received from M. Guidal as a private communication.
 */
class DVMPProcessGK06: public DVMPProcessModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVMPProcessGK06(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~DVMPProcessGK06();
    virtual DVMPProcessGK06* clone() const;

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    DVMPProcessGK06(const DVMPProcessGK06& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSection();

private:

    double CrossSectionL(); ///< Partial cross-section L
    double CrossSectionT(); ///< Partial cross-section T
    double CrossSectionLT(); ///< Partial cross-section LT
    double CrossSectionTT(); ///< Partial cross-section TT

    double m_W2; ///< W^2 variable.
    double m_xi; ///< xi variable.

    double lambdaFunction(double a, double b, double c) const; ///< Mandalstam function.
};

} /* namespace PARTONS */

#endif /* DVMP_PROCESS_VGG99_H */
