#ifndef GPD_GK19_H
#define GPD_GK19_H

/**
 * @file GPDGK19.h
 * @author Kemal Tezgin
 * @date 2019
 * @version 1.0
 */

#include <string>

#include "GPDGK16.h"

namespace PARTONS {

/**
 * @class GPDGK19
 *
 * @brief TODO A brief description comes here.
 *
 * TODO Extended description comes here.
 *
 */
class GPDGK19: public GPDGK16 {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDGK19(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDGK19();

    virtual GPDGK19* clone() const;

protected:

    /**
     * Copy constructor.
     */
    GPDGK19(const GPDGK19& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();
    virtual PartonDistribution computeHTrans();
    virtual PartonDistribution computeETrans();

private:

};

} /* namespace PARTONS */

#endif /* GPD_GK19_H */
