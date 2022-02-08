#ifndef DDVCS_XI_CONVERTER_TEST_H
#define DDVCS_XI_CONVERTER_TEST_H

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DDVCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class DDVCSXiConverterTEST
 *
 * @brief TODO
 *
 * TODO
 */
class DDVCSXiConverterTEST: public DDVCSXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSXiConverterTEST(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSXiConverterTEST(const DDVCSXiConverterTEST &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSXiConverterTEST();

    virtual DDVCSXiConverterTEST* clone() const;
    virtual PhysicalType<double> compute(
            const DDVCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DDVCS_XI_CONVERTER_TEST_H */
