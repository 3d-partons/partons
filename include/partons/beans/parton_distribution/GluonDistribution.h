#ifndef GLUON_DISTRIBUTION_H
#define GLUON_DISTRIBUTION_H

/**
 * @file QuarkDistribution.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 April 2015
 * @version 1.0
 */

#include <string>

#include "../../BaseObject.h"
#include "../../utils/compare/ComparisonReport.h"

class ComparisonReport;

/**
 * @class GluonDistribution
 *
 * @brief Container to store value of single gluon distribution.
 *
 * This class represents a gluon distribution at a single kinematic point. For example, it can be a gluon GPD of a given type at some GPD kinematics.
 *
 * To see how this class can be used, analyze the following example:
 \code{.cpp}
 //in code, e.g. in one of GPD modules, one calculates value of GPD H for gluons at given kinematics
 double Hg = 3.45;

 //store this result in GluonDistribution object
 GluonDistribution gluonDistribution(Hg);

 //check what is inside
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Gluon distribution contains: " << gluonDistribution.toString());
 \endcode
 which gives via Logger:
 \code
 20-05-2017 08:51:19 [INFO] (example::main) Gluon distribution contains: GluonDistribution = 3.45
 \endcode
 * Check also the documentation of GPDResult and PartonDistribution classes, where GluonDistribution objects are used extensively.
 */
class GluonDistribution: public BaseObject {

public:

    /**
     * Name of field in the database storing value of gluon distribution.
     */
    static const std::string GLUON_DISTRIBUTION_DB_COLUMN_NAME;

    /**
     *
     */
    static const std::string GLUON_DISTRIBUTION_PARAMETER_NAME_GLUON_DISTRIBUTION;

    /**
     * Default constructor.
     */
    GluonDistribution();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GluonDistribution(const GluonDistribution &other);

    /**
     * Assignment constructor.
     * @param gluonDistribution Value to be assigned.
     */
    GluonDistribution(double gluonDistribution);

    /**
     * Destructor.
     */
    virtual ~GluonDistribution();

    virtual std::string toString() const;

    /**
     * Compare to other GluonDistribution object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const GluonDistribution &referenceObject,
            std::string parentObjectInfo = "") const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get value of gluon distribution.
     */
    double getGluonDistribution() const;

    /**
     * Set value of gluon distribution.
     */
    void setGluonDistribution(double gluonDistribution);

    /**
     * Check if any value of gluon distribution has been set.
     */
    bool isNullObject() const;

    /**
     * Set GluonDistribution::m_nullObject.
     */
    void setNullObject(bool nullObject);

private:

    /**
     * Value of gluon distribution.
     */
    double m_gluonDistribution;

    /**
     * Variable to check if any value of gluon distribution has been set.
     */
    bool m_nullObject;
};

#endif /* GLUON_DISTRIBUTION_H */
