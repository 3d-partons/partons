#ifndef DVCS_PROCESS_BMP14_H
#define DVCS_PROCESS_BMP14_H

/**
 * @file DVCSProcessBMP14.h
 * @author Victor MARTINEZ-FERNANDEZ (NCBJ / Warsaw) & Pawel SZNAJDER (NCBJ / Warsaw)
 * @date year 2023
 * @version
 */

#include <string>

#include "DVCSProcessBMJ12.h"

namespace PARTONS {

/**
 * @class DVCSProcessBMP14
 *
 * TODO
 */
class DVCSProcessBMP14: public DVCSProcessBMJ12 {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVCSProcessBMP14(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~DVCSProcessBMP14();

    virtual DVCSProcessBMP14* clone() const;

protected:

    /**
     * Copy constructor.
     *
     * Used by the factory.
     *
     * @param other
     */
    DVCSProcessBMP14(const DVCSProcessBMP14& other);

private:

    void computeCFFs(); ///< Computes CFFS F+b and F0+.
};

} /* namespace PARTONS */

#endif /* DVCS_PROCESS_BMP14_H */
