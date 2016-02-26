#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"

#include <sstream>


const std::string GluonDistribution::GLUON_DISTRIBUTION_DB_COLUMN_NAME =
        "gluon_distribution";

const std::string GluonDistribution::GLUON_DISTRIBUTION_PARAMETER_NAME_GLUON_DISTRIBUTION =
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

//ComparisonReport GluonDistribution::compare(
//        const GluonDistribution& referenceObject,
//        const NumA::Tolerances& tolerances) const {
//
//    ComparisonReport comparisonReport(getClassName());
//
//    ComparisonData gluon_comparisonData =
//            CompareUtils::compareDouble(
//                    GluonDistribution::GLUON_DISTRIBUTION_PARAMETER_NAME_GLUON_DISTRIBUTION,
//                    getGluonDistribution(),
//                    referenceObject.getGluonDistribution(), tolerances);
//    comparisonReport.addComparisonData(gluon_comparisonData);
//
//    return comparisonReport;
//}
