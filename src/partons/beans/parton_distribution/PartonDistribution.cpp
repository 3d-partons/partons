#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

PartonDistribution::PartonDistribution() :
        BaseObject("PartonDistribution") {
}

PartonDistribution::PartonDistribution(const PartonDistribution &other) :
        BaseObject(other) {
    m_gluonDistribution = other.m_gluonDistribution;
    m_quarkDistributions = other.m_quarkDistributions;
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
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Unable to find QuardDistribution object from QuarFlavorType = "
                        << QuarkFlavor(quarkFlavorType).getShortName());
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

List<QuarkDistribution> PartonDistribution::getListOfQuarkDistribution() const {
    List<QuarkDistribution> result;

    for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            m_quarkDistributions.begin(); it != m_quarkDistributions.end();
            it++) {
        result.add(it->second);
    }

    return result;
}

std::string PartonDistribution::toString() const {
    ElemUtils::Formatter formatter;

    formatter << BaseObject::toString() << '\n';

    formatter << m_gluonDistribution.toString();

    for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            m_quarkDistributions.begin(); it != m_quarkDistributions.end();
            it++) {
        formatter << (it->second).toString();
    }

    return formatter.str();
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
//           throw ElemUtils::CustomException(getClassName,__func__,
//                   "Divided by ZERO !");
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
            std::make_pair(quarkDistribution.getQuarkFlavor().getType(),
                    quarkDistribution));
}

void PartonDistribution::compare(ComparisonReport &rootComparisonReport,
        const PartonDistribution &referenceObject,
        std::string parentObjectInfo) const {

    // compare gluon distribution
    this->m_gluonDistribution.compare(rootComparisonReport,
            referenceObject.getGluonDistribution(),
            ElemUtils::Formatter() << parentObjectInfo);

    for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            m_quarkDistributions.begin(); it != m_quarkDistributions.end();
            it++) {
        (it->second).compare(rootComparisonReport,
                referenceObject.getQuarkDistribution((it->first)),
                ElemUtils::Formatter() << parentObjectInfo << " "
                        << QuarkFlavor(it->first).toString());
    }
}
