#include "GluonDistribution.h"

#include <sstream>

const std::string GluonDistribution::GLUON_DISTRIBUTION_DB_COLUMN_NAME = "gluon_distribution";

GluonDistribution::GluonDistribution()
        : m_gluonDistribution(0.) {
}

GluonDistribution::GluonDistribution(double gluonDistribution)
        : m_gluonDistribution(gluonDistribution) {
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
}
