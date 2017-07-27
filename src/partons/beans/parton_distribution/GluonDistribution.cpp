#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"

namespace PARTONS {


const std::string GluonDistribution::GLUON_DISTRIBUTION_DB_COLUMN_NAME =
        "gluon_distribution";

const std::string GluonDistribution::GLUON_DISTRIBUTION_PARAMETER_NAME_GLUON_DISTRIBUTION =
        "gluon_distribution";

GluonDistribution::GluonDistribution() :
        BaseObject("GluonDistribution"), m_gluonDistribution(0.), m_nullObject(
                true) {
}

GluonDistribution::GluonDistribution(const GluonDistribution &other) :
        BaseObject(other) {
    m_gluonDistribution = other.m_gluonDistribution;
    m_nullObject = other.m_nullObject;
}

GluonDistribution::GluonDistribution(double gluonDistribution) :
        BaseObject("GluonDistribution"), m_gluonDistribution(gluonDistribution), m_nullObject(
                false) {
}

GluonDistribution::~GluonDistribution() {

}

std::string GluonDistribution::toString() const {
    return ElemUtils::Formatter() << "GluonDistribution = "
            << m_gluonDistribution << '\n';
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

void GluonDistribution::compare(ComparisonReport &rootComparisonReport,
        const GluonDistribution &referenceObject,
        std::string parentObjectInfo) const {
    ComparisonData gluon_value_comparisonData =
            CompareUtils::compareDouble(
                    GluonDistribution::GLUON_DISTRIBUTION_PARAMETER_NAME_GLUON_DISTRIBUTION,
                    m_gluonDistribution, referenceObject.getGluonDistribution(),
                    rootComparisonReport.getTolerances(),
                    ElemUtils::Formatter() << parentObjectInfo << " "
                            << getClassName());

    rootComparisonReport.addComparisonData(gluon_value_comparisonData);
}

} /* namespace PARTONS */
