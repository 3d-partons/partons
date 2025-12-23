#ifndef JET_PROCESS_PSSW26_H
#define JET_PROCESS_PSSW26_H

/**
 * @file JETProcessPSSW26.h
 * @author Pawel Sznajder (NCBJ)
 * @date 23 December 2025
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "JETProcessModule.h"

namespace PARTONS {

/**
 * @class JETProcessPSSW26
 *
 */
class JETProcessPSSW26: public JETProcessModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className name of child class.
     */
    JETProcessPSSW26(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~JETProcessPSSW26();

    virtual JETProcessPSSW26* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETProcessPSSW26(const JETProcessPSSW26& other);

    virtual PhysicalType<double> CrossSection();

private:

    /**
     * Alpha_s (2 loop)
     * @param mu2 Scale.
     */
    double alphaS_2loop(double mu2) const;
};

} /* namespace PARTONS */

#endif /* JET_PROCESS_PSSW26_H */
