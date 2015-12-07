#include "GluonDistribution.h"

#include <sstream>

#include "../../utils/test/ComparisonReport.h"
#include "../ComparisonData.h"

const std::string GluonDistribution::GLUON_DISTRIBUTION_DB_COLUMN_NAME =
        "gluon_distribution";

GluonDistribution::GluonDistribution() :
        BaseObject("GluonDistribution"), m_gluonDistribution(0.), m_nullObject(
                true) {
}

GluonDistribution::GluonDistribution(double gluonDistribution) :
        BaseObject("GluonDistribution"), m_gluonDistribution(gluonDistribution), m_nullObject(
                false) {
}

GluonDistribution::~GluonDistribution() {

}

std::string GluonDistribution::toString() const {
    std::ostringstream os;

    os << "GluonDistribution = " << m_gluonDistribution << std::endl;

    return os.str();
}

double GluonDistribution::getGluonDistribution() const {
    return m_gluonDistribution;
}

void GluonDistribution::setGluonDistribution(double gluonDistribution) {
    m_gluonDistribution = gluonDistribution;
    m_nullObject = false;
}

bool GluonDistribution::isNullObject() const {
    return m_nullObject;
}

void GluonDistribution::setNullObject(bool nullObject) {
    m_nullObject = nullObject;
}

//TODO implement
ComparisonReport GluonDistribution::compare(
        const GluonDistribution& referenceObject,
        const Tolerances& tolerances) const {

    ComparisonReport comparisonReport(getClassName());

    //TODO replace column name by real variable name
    comparisonReport.addComparisonData(
            ComparisonData(
                    GluonDistribution::GLUON_DISTRIBUTION_DB_COLUMN_NAME));

    return comparisonReport;
}
