#ifndef COLLINEAR_DISTRIBUTION_MODULE_H
#define COLLINEAR_DISTRIBUTION_MODULE_H

/**
 * @file CollinearDistributionModule.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date July 17, 2020
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/List.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../beans/parton_distribution/PartonDistributionKinematic.h"
#include "../../ModuleObject.h"

namespace PARTONS {
class PartonDistributionKinematic;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class CollinearDistributionModule @brief Abstract class that
 * provides a skeleton to implement a collinear-distribution module
 * (such as polarised and unpolarise PDFs and FFs).
 */
class CollinearDistributionModule: public ModuleObject {

public:

    static const std::string COLLINEAR_DISTRIBUTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionModule();

    virtual CollinearDistributionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Virtual method, computes CollinearDistribution with some input parameters.
     *
     * @param kinematic CollinearDistribution kinematics object.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution compute(const PartonDistributionKinematic &kinematic);

protected:

    /**
     * Default constructor.
     */
    CollinearDistributionModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    CollinearDistributionModule(const CollinearDistributionModule &other);

    /**
     * Set internal kinematics.
     * @param kinematic Kinematics to be set.
     */
    virtual void setKinematics(const PartonDistributionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * Fuctions to be implemented in the child class that actually
     * returs the set of collinear distributions in the form of a
     * PartonDistribution object.
     */
    virtual PartonDistribution compute() const = 0;

    double m_x;    ///< Longitudinal momentum fraction of the active parton.
    double m_MuF2; ///< Factorization scale squared.
    double m_MuR2; ///< Renormalization scale squared.
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_MODULE_H */
