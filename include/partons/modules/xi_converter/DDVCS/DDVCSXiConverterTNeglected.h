#ifndef DDVCS_XI_CONVERTER_T_NEGLECTED_H
#define DDVCS_XI_CONVERTER_T_NEGLECTED_H

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DDVCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class DDVCSXiConverterTEST
 *
 * @brief Evaluation of skewness with t=0 simplification.
 *
 * The module evaluates xi according to:
 * xi = (Q2 + Q2Prim) / (2. * Q2 / xB - Q2 - Q2Prim)
 *
 * t-dependence is neglected here.
 */
class DDVCSXiConverterTNeglected: public DDVCSXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSXiConverterTNeglected(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSXiConverterTNeglected(const DDVCSXiConverterTNeglected &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSXiConverterTNeglected();

    virtual DDVCSXiConverterTNeglected* clone() const;
    virtual PhysicalType<double> compute(
            const DDVCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DDVCS_XI_CONVERTER_T_NEGLECTED_H */
