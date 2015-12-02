#include "PartonDistribution.h"

#include <sstream>
#include <utility>

#include "../../utils/test/ComparisonReport.h"

PartonDistribution::PartonDistribution() :
        BaseObject("PartonDistribution") {
}

PartonDistribution::~PartonDistribution() {
}

void PartonDistribution::addQuarkDistribution(
        const QuarkDistribution &quarkDistribution) {
    m_quarkDistributions.insert(
            std::make_pair(quarkDistribution.getQuarkFlavor(),
                    quarkDistribution));
}

const QuarkDistribution& PartonDistribution::getQuarkDistribution(
        QuarkFlavor::Type quarkFlavorType) const {

    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            m_quarkDistributions.find(quarkFlavorType);

    if (it == m_quarkDistributions.end()) {
        throwException(__func__,
                "Enable to find QuardDistribution object from QuarFlavorType = "
                        + QuarkFlavor(quarkFlavorType).getShortName());
    }

    return (it->second);
}

std::vector<QuarkFlavor::Type> PartonDistribution::listTypeOfQuarkFlavor() {
    std::vector<QuarkFlavor::Type> list;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it;

    if (m_quarkDistributions.size() != 0) {

        for (it = m_quarkDistributions.begin();
                it != m_quarkDistributions.end(); ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

unsigned int PartonDistribution::getQuarkDistributionsSize() const {
    return m_quarkDistributions.size();
}

const std::vector<QuarkDistribution> PartonDistribution::getVectorOfQuarkDistribution() const {
    std::vector<QuarkDistribution> result;
    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it;

    for (it = m_quarkDistributions.begin(); it != m_quarkDistributions.end();
            it++) {
        result.push_back(it->second);
    }

    return result;
}

std::string PartonDistribution::toString() const {
    std::ostringstream os;

    os << m_gluonDistribution.toString();

    for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            m_quarkDistributions.begin(); it != m_quarkDistributions.end();
            it++) {
        os << (it->second).toString();
    }

    return os.str();
}

const GluonDistribution& PartonDistribution::getGluonDistribution() const {
    return m_gluonDistribution;
}

void PartonDistribution::setGluonDistribution(
        const GluonDistribution& gluonDistribution) {
    m_gluonDistribution = gluonDistribution;
}

const std::map<QuarkFlavor::Type, QuarkDistribution>& PartonDistribution::getQuarkDistributions() const {
    return m_quarkDistributions;
}

void PartonDistribution::setQuarkDistributions(
        const std::map<QuarkFlavor::Type, QuarkDistribution>& quarkDistributions) {
    m_quarkDistributions = quarkDistributions;
}

double PartonDistribution::getSinglet() {
    double result = 0.;

    for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            m_quarkDistributions.begin(); it != m_quarkDistributions.end();
            it++) {
        result += (it->second).getQuarkDistributionPlus();
    }

//    if (m_nbOfQuarkFlavor == 0) {
//           throw std::runtime_error(
//                   "[GK11Model::computeSinglet] divided by ZERO !");
//       }
//
//       result = quarkDistribution_u.getQuarkDistributionPlus()
//               + quarkDistribution_d.getQuarkDistributionPlus()
//               + quarkDistribution_s.getQuarkDistributionPlus();
//       result *= (1 / (2 * m_nbOfQuarkFlavor));

    return result;
}

void PartonDistribution::addQuarkDistribution(
        QuarkDistribution& quarkDistribution) {
    m_quarkDistributions.insert(
            std::make_pair(quarkDistribution.getQuarkFlavor(),
                    quarkDistribution));
}

ComparisonReport PartonDistribution::compare(const PartonDistribution& other,
        const Tolerances& tolerances) const {

    ComparisonReport comparisonReport(getClassName(), toString());

    for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator m_it =
            m_quarkDistributions.begin(); m_it != m_quarkDistributions.end();
            m_it++) {
        comparisonReport.addChildren(
                (m_it->second).compare(
                        other.getQuarkDistribution((m_it->first)), tolerances));
    }

    return comparisonReport;
}
