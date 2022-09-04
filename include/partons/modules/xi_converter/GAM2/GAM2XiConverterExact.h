#ifndef GAM2_XI_CONVERTER_XB_TO_XI_H
#define GAM2_XI_CONVERTER_XB_TO_XI_H

/**
 * @file GAM2XiConverterExact.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "../../../utils/type/PhysicalType.h"
#include "GAM2XiConverterModule.h"

namespace PARTONS {

/**
 * @class GAM2XiConverterExact
 *
 * @brief Evaluation of GPD variable xi from kinematics for GAM2.
 *
 * \f$\xi = (M_{\gamma\gamma}^{2} - t)/(2 s)\f$
 */
class GAM2XiConverterExact: public GAM2XiConverterModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    GAM2XiConverterExact(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2XiConverterExact(const GAM2XiConverterExact &other);

    /**
     * Destructor.
     */
    virtual ~GAM2XiConverterExact();

    virtual GAM2XiConverterExact* clone() const;
    virtual PhysicalType<double> compute(
            const GAM2ObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* GAM2_XI_CONVERTER_XB_TO_XI_H */
