#ifndef DDVCS_XI_CONVERTER_TMIN_H
#define DDVCS_XI_CONVERTER_TMIN_H

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "DDVCSXiConverterModule.h"

namespace PARTONS {

/**
 * @class DDVCSXiConverterTMin
 *
 * @brief TODO
 *
 * TODO
 */
class DDVCSXiConverterTMin: public DDVCSXiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSXiConverterTMin(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSXiConverterTMin(const DDVCSXiConverterTMin &other);

    /**
     * Destructor.
     */
    virtual ~DDVCSXiConverterTMin();

    virtual DDVCSXiConverterTMin* clone() const;
    virtual PhysicalType<double> compute(
            const DDVCSObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DDVCS_XI_CONVERTER_TMIN_H */
