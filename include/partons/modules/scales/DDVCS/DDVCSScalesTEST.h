#ifndef DDVCS_SCALES_TEST_H
#define DDVCS_SCALES_TEST_H

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "DDVCSScalesModule.h"

namespace PARTONS {

/**
 * @class DDVCSScalesTEST
 *
 * @brief TODO
 *
 * TODO
 */
class DDVCSScalesTEST: public DDVCSScalesModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSScalesTEST(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSScalesTEST(const DDVCSScalesTEST &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSScalesTEST();

    virtual DDVCSScalesTEST* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual Scales compute(const DDVCSObservableKinematic& kinematic);

private:

};

} /* namespace PARTONS */

#endif /* DDVCS_SCALES_TEST_H */
