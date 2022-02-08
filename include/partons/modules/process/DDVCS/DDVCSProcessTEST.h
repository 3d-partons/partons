#ifndef DDVCS_PROCESS_GV08_H
#define DDVCS_PROCESS_GV08_H

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DDVCSProcessModule.h"

namespace PARTONS {

/**
 * @class DDVCSProcessTEST
 *
 * @brief TODO
 *
 * TODO
 */
class DDVCSProcessTEST: public DDVCSProcessModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DDVCSProcessTEST(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DDVCSProcessTEST();

    virtual DDVCSProcessTEST* clone() const;

protected:

    /**
     * Copy constructor.
     */
    DDVCSProcessTEST(const DDVCSProcessTEST& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual PhysicalType<double> CrossSectionBH();
    virtual PhysicalType<double> CrossSectionVCS();
    virtual PhysicalType<double> CrossSectionInterf();

private:

};

} /* namespace PARTONS */

#endif /* DDVCS_PROCESS_GV08_H */

