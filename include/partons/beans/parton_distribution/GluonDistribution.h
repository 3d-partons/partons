#ifndef GLUON_DISTRIBUTION_H
#define GLUON_DISTRIBUTION_H

#include <string>

#include "../../BaseObject.h"

class ComparisonReport;

namespace NumA {
class Tolerances;
} /* namespace NumA */

class GluonDistribution: public BaseObject {
public:
    static const std::string GLUON_DISTRIBUTION_DB_COLUMN_NAME;

    static const std::string GLUON_DISTRIBUTION_PARAMETER_NAME_GLUON_DISTRIBUTION;

    GluonDistribution();
    GluonDistribution(double gluonDistribution);
    virtual ~GluonDistribution();

    virtual std::string toString() const;

    void compare(ComparisonReport &rootComparisonReport,
            const GluonDistribution &referenceObject,
            const NumA::Tolerances &tolerances, std::string parentObjectInfo =
                    "") const;

    // ##### GETTERS & SETTERS #####

    double getGluonDistribution() const;
    void setGluonDistribution(double gluonDistribution);
    bool isNullObject() const;
    void setNullObject(bool nullObject);

private:
    double m_gluonDistribution;
    bool m_nullObject;
};

#endif /* GLUON_DISTRIBUTION_H */
