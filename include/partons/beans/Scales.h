#ifndef SCALES_H
#define SCALES_H

/**
 * @file Scales.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../utils/type/PhysicalType.h"

namespace PARTONS {

/**
 * @class Scales
 *
 * @brief Container to store square values of factorization and renormalization scales.
 */
class Scales: public BaseObject {

public:

    /**
     * Default constructor.
     */
    Scales();

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2.
     */
    Scales(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     * @param MuF2 Squared value of factorization scale to be set.
     * @param MuR2 Squared value of renormalization scale to be set.
     */
    Scales(double MuF2, double MuR2);

    /**
     * Assignment constructor.
     * @param MuF2 Squared value of factorization scale to be set.
     * @param MuR2 Squared value of renormalization scale to be set.
     */
    Scales(const PhysicalType<double>& MuF2, const PhysicalType<double>& MuR2);

    /**
     * Assignment constructor.
     * @param MuF2 Squared value of factorization scale to be set.
     * @param MuR2 Squared value of renormalization scale to be set.
     */
    Scales(const ElemUtils::GenericType& MuF2,
            const ElemUtils::GenericType& MuR2);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Scales(const Scales &other);

    /**
     * Destructor.
     */
    virtual ~Scales();

    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get squared value of factorization scale.
     */
    const PhysicalType<double>& getMuF2() const;

    /**
     * Set squared value of factorization scale.
     */
    void setMuF2(const PhysicalType<double>& muF2);

    /**
     * Set squared value of factorization scale.
     */
    void setMuF2(double muF2);

    /**
     * Get squared value of renormalization scale.
     */
    const PhysicalType<double>& getMuR2() const;

    /**
     * Set squared value of renormalization scale.
     */
    void setMuR2(double muR2);

    /**
     * Set squared value of renormalization scale.
     */
    void setMuR2(const PhysicalType<double>& muR2);

private:

    /**
     * Squared value of factorization scale.
     */
    PhysicalType<double> m_MuF2;

    /**
     * Squared value of renormalization scale.
     */
    PhysicalType<double> m_MuR2;
};

} /* namespace PARTONS */

#endif /* SCALES_H */
