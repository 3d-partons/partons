#include "PartonDistribution.h"

#include <sstream>
#include <utility>

PartonDistribution::PartonDistribution() :
        BaseObject("PartonDistribution"), m_singlet(0.) {
}

PartonDistribution::~PartonDistribution() {
}

void PartonDistribution::addQuarkDistribution(
        QuarkDistribution &quarkDistribution) {
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
    os << "Singlet = " << m_singlet << std::endl;

    std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it;

    for (it = m_quarkDistributions.begin(); it != m_quarkDistributions.end();
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

double PartonDistribution::getSinglet() const {
    return m_singlet;
}

void PartonDistribution::setSinglet(double singlet) {
    m_singlet = singlet;
}
