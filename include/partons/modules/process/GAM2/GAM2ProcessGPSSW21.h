#ifndef GAM2_PROCESS_GV08_H
#define GAM2_PROCESS_GV08_H

/**
 * @file GAM2ProcessGPSSW21.h
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @version 1.0
 *
 * @date 25-09-2015 (Bryan BERTHOU) : refactoring
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "GAM2ProcessModule.h"

namespace PARTONS {

/**
 * @class GAM2ProcessGPSSW21
 *
 * Module for the GAM2 process using arXiv:2110.00048 set of formulas.
 */
class GAM2ProcessGPSSW21: public GAM2ProcessModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GAM2ProcessGPSSW21(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GAM2ProcessGPSSW21();

    virtual GAM2ProcessGPSSW21* clone() const;

protected:

    /**
     * Copy constructor.
     */
    GAM2ProcessGPSSW21(const GAM2ProcessGPSSW21& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSection();

private:

};

} /* namespace PARTONS */

#endif /* GAM2_PROCESS_GV08_H */

