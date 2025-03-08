#ifndef DDVCS_SCALES_VIRTUALITIES_SUM_H
#define DDVCS_SCALES_VIRTUALITIES_SUM_H

#include <string>

#include "DDVCSScalesModule.h"

namespace PARTONS {

/**
 * @class DDVCSScalesTEST
 *
 * @brief Evaluation of scales for DDVCS module as sum of virtualities.
 *
 * Both scales are returned as sum of virtualities.
 */
class DDVCSScalesVirtualitiesSum: public DDVCSScalesModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSScalesVirtualitiesSum(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSScalesVirtualitiesSum(const DDVCSScalesVirtualitiesSum &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSScalesVirtualitiesSum();

    virtual DDVCSScalesVirtualitiesSum* clone() const;
    virtual Scales compute(const DDVCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DDVCS_SCALES_VIRTUALITIES_SUM_H */
