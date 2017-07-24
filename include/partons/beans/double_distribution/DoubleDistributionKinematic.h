#ifndef DOUBLE_DISTRIBUTION_KINEMATIC_H
#define DOUBLE_DISTRIBUTION_KINEMATIC_H

/**
 * @file DoubleDistributionKinematic.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 29 July 2015
 * @version 1.0
 */

#include "DoubleDistributionType.h"

namespace PARTONS {

/**
 *  @class DoubleDistributionKinematic
 *
 * @brief
 */
class DoubleDistributionKinematic {
public:
    DoubleDistributionKinematic();
    DoubleDistributionKinematic(double beta, double alpha, double t,
            double MuF2, double MuR2,
            DoubleDistributionType::Type doubleDistributionType);

    virtual ~DoubleDistributionKinematic();

    // ##### GETTERS & SETTERS #####

    double getAlpha() const;
    void setAlpha(double alpha);
    double getBeta() const;
    void setBeta(double beta);
    double getMuF2() const;
    void setMuF2(double muF2);
    double getMuR2() const;
    void setMuR2(double muR2);
    double getT() const;
    void setT(double t);
    const DoubleDistributionType& getDoubleDistributionType() const;
    void setDoubleDistributionType(
            const DoubleDistributionType& doubleDistributionType);

private:

    double m_beta;
    double m_alpha;
    double m_t;
    double m_MuF2;
    double m_MuR2;

    DoubleDistributionType m_doubleDistributionType;
};

} /* namespace PARTONS */

#endif /* DOUBLE_DISTRIBUTION_KINEMATIC_H */
