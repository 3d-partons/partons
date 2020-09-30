#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/collinear_distribution/CollinearDistributionLHAPDF.h"

namespace PARTONS {

const std::string CollinearDistributionLHAPDF::PARAM_NAME_SET_NAME = "setName";
const std::string CollinearDistributionLHAPDF::PARAM_NAME_SET_TYPE = "setType";
const std::string CollinearDistributionLHAPDF::PARAM_NAME_SET_MEMBER =
        "setMember";

const unsigned int CollinearDistributionLHAPDF::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CollinearDistributionLHAPDF("CollinearDistributionLHAPDF"));

//TODO initialise missing members
CollinearDistributionLHAPDF::CollinearDistributionLHAPDF(
        const std::string &className) :
        CollinearDistributionModule(className), m_setName("UNDEFINED"), m_member(
                0), m_type(CollinearDistributionType::Type::UNDEFINED), m_set(
                { }) {

}

CollinearDistributionLHAPDF::CollinearDistributionLHAPDF(
        const CollinearDistributionLHAPDF& other) :
        CollinearDistributionModule(other) {

    m_setName = other.getSetName();
    m_member = other.getMember();
    m_type = other.getType();
    m_set = other.getSet();
}

CollinearDistributionLHAPDF* CollinearDistributionLHAPDF::clone() const {
    return new CollinearDistributionLHAPDF(*this);
}

CollinearDistributionLHAPDF::~CollinearDistributionLHAPDF() {
}

void CollinearDistributionLHAPDF::configure(
        const ElemUtils::Parameters &parameters) {

    CollinearDistributionModule::configure(parameters);

    // LHAPDF in silent mode
    LHAPDF::setVerbosity(0);

    //check and set
    if (parameters.isAvailable(
            CollinearDistributionLHAPDF::PARAM_NAME_SET_NAME)) {
        setSetName(parameters.getLastAvailable().getString());
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionLHAPDF::PARAM_NAME_SET_NAME
                        << " configured with value = " << getSetName());
    }

    if (parameters.isAvailable(
            CollinearDistributionLHAPDF::PARAM_NAME_SET_MEMBER)) {
        setMember(parameters.getLastAvailable().toUInt());
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionLHAPDF::PARAM_NAME_SET_MEMBER
                        << " configured with value = " << getMember());
    }

    if (parameters.isAvailable(
            CollinearDistributionLHAPDF::PARAM_NAME_SET_TYPE)) {
        setType(
                CollinearDistributionType { }.fromString(
                        parameters.getLastAvailable().getString()));
        info(__func__,
                ElemUtils::Formatter()
                        << CollinearDistributionLHAPDF::PARAM_NAME_SET_TYPE
                        << " configured with value = "
                        << CollinearDistributionType(getType()).toString());
    }
}

void CollinearDistributionLHAPDF::isModuleWellConfigured() {

    CollinearDistributionModule::isModuleWellConfigured();

    //check that the set name in no UNDEFINED
    if (m_setName == "UNDEFINED") {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "The set name is undefined");
    }

    //check that the member index is non-negative
    if (m_member < 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "The member index is negative");
    }

    if (m_type == CollinearDistributionType::Type::UNDEFINED) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "The collinear distribution type is undefined");
    }
}

void CollinearDistributionLHAPDF::initModule() {

    CollinearDistributionModule::initModule();

    if (m_set.empty()) {
        m_set = LHAPDF::mkPDFs(m_setName);
    }
}

PartonDistribution CollinearDistributionLHAPDF::computeUnpolPDF() {
    PartonDistribution partonDistribution;

    // Return empty distribution is the type does not match with the
    // function.
    if (m_type != CollinearDistributionType::UnpolPDF)
        return partonDistribution;

    // Get distributions from LHAPDF (only positive values of x are allowed)
    const std::map<int, double> dists = m_set[m_member]->xfxQ2(std::abs(m_x),
            m_MuF2);

    // Map between QuarkFlavor enum and GPD convention for quarks
    const std::map<int, QuarkFlavor::Type> flavour_map {
            { 1, QuarkFlavor::DOWN }, { 2, QuarkFlavor::UP }, { 3,
                    QuarkFlavor::STRANGE }, { 4, QuarkFlavor::CHARM }, { 5,
                    QuarkFlavor::BOTTOM }, { 6, QuarkFlavor::TOP } };

    // Fill in map of QuarkDistribution
    std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributions;
    for (auto const& d : flavour_map) {

        // If the flavour isn't there continue
        if (dists.find(d.first) == dists.end()
                || dists.find(-d.first) == dists.end())
            continue;

        // Determine quark flavour
        const QuarkFlavor::Type type = flavour_map.at(std::abs(d.first));

        // Construct relevant combinations
        const double qrk = (m_x > 0 ? dists.at(d.first) : -dists.at(-d.first));
        const double qrkPlus = (
                m_x > 0 ?
                        dists.at(d.first) + dists.at(-d.first) :
                        -dists.at(-d.first) - dists.at(d.first));
        const double qrkMinus = (
                m_x > 0 ?
                        dists.at(d.first) - dists.at(-d.first) :
                        -dists.at(-d.first) + dists.at(d.first));
        quarkDistributions.insert( { type, QuarkDistribution { type, qrk,
                qrkPlus, qrkMinus } });
    }

    // Put quark distributions into the PartonDistribution object
    partonDistribution.setQuarkDistributions(quarkDistributions);

    // Now set the gluon
    partonDistribution.setGluonDistribution(GluonDistribution { dists.at(21) });

    // Return
    return partonDistribution;
}

PartonDistribution CollinearDistributionLHAPDF::computePolPDF() {
    PartonDistribution partonDistribution;

    // Return empty distribution is the type does not match with the
    // function.
    if (m_type != CollinearDistributionType::PolPDF)
        return partonDistribution;

    // Get distributions from LHAPDF (only positive values of x are allowed)
    const std::map<int, double> dists = m_set[m_member]->xfxQ2(std::abs(m_x),
            m_MuF2);

    // Map between QuarkFlavor enum and GPD convention for quarks
    const std::map<int, QuarkFlavor::Type> flavour_map {
            { 1, QuarkFlavor::DOWN }, { 2, QuarkFlavor::UP }, { 3,
                    QuarkFlavor::STRANGE }, { 4, QuarkFlavor::CHARM }, { 5,
                    QuarkFlavor::BOTTOM }, { 6, QuarkFlavor::TOP } };

    // Fill in map of QuarkDistribution
    std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributions;
    for (auto const& d : flavour_map) {

        // If the flavour isn't there continue
        if (dists.find(d.first) == dists.end()
                || dists.find(-d.first) == dists.end())
            continue;

        // Determine quark flavour
        const QuarkFlavor::Type type = flavour_map.at(std::abs(d.first));

        // Construct relevant combinations
        const double qrk = (m_x > 0 ? dists.at(d.first) : dists.at(-d.first));
        const double qrkPlus = (
                m_x > 0 ?
                        dists.at(d.first) - dists.at(-d.first) :
                        dists.at(-d.first) - dists.at(d.first));
        const double qrkMinus = (
                m_x > 0 ?
                        dists.at(d.first) + dists.at(-d.first) :
                        dists.at(-d.first) + dists.at(d.first));
        quarkDistributions.insert( { type, QuarkDistribution { type, qrk,
                qrkPlus, qrkMinus } });
    }

    // Put quark distributions into the PartonDistribution object
    partonDistribution.setQuarkDistributions(quarkDistributions);

    // Now set the gluon
    partonDistribution.setGluonDistribution(
            GluonDistribution { (m_x > 0 ? dists.at(21) : -dists.at(21)) });

    // Return
    return partonDistribution;
}

PartonDistribution CollinearDistributionLHAPDF::computeUnpolFF() {
    PartonDistribution partonDistribution;

    // Return empty distribution is the type does not match with the
    // function.
    if (m_type != CollinearDistributionType::UnpolFF)
        return partonDistribution;

    // Get distributions from LHAPDF (only positive values of x are allowed)
    const std::map<int, double> dists = m_set[m_member]->xfxQ2(std::abs(m_x),
            m_MuF2);

    // Map between QuarkFlavor enum and GPD convention for quarks
    const std::map<int, QuarkFlavor::Type> flavour_map {
            { 1, QuarkFlavor::DOWN }, { 2, QuarkFlavor::UP }, { 3,
                    QuarkFlavor::STRANGE }, { 4, QuarkFlavor::CHARM }, { 5,
                    QuarkFlavor::BOTTOM }, { 6, QuarkFlavor::TOP } };

    // Fill in map of QuarkDistribution
    std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributions;
    for (auto const& d : flavour_map) {

        // If the flavour isn't there continue
        if (dists.find(d.first) == dists.end()
                || dists.find(-d.first) == dists.end())
            continue;

        // Determine quark flavour
        const QuarkFlavor::Type type = flavour_map.at(std::abs(d.first));

        // Construct relevant combinations
        const double qrk = (m_x > 0 ? dists.at(d.first) : -dists.at(-d.first));
        const double qrkPlus = (
                m_x > 0 ?
                        dists.at(d.first) + dists.at(-d.first) :
                        -dists.at(-d.first) - dists.at(d.first));
        const double qrkMinus = (
                m_x > 0 ?
                        dists.at(d.first) - dists.at(-d.first) :
                        -dists.at(-d.first) + dists.at(d.first));
        quarkDistributions.insert( { type, QuarkDistribution { type, qrk,
                qrkPlus, qrkMinus } });
    }

    // Put quark distributions into the PartonDistribution object
    partonDistribution.setQuarkDistributions(quarkDistributions);

    // Now set the gluon
    partonDistribution.setGluonDistribution(GluonDistribution { dists.at(21) });

    // Return
    return partonDistribution;
}

PartonDistribution CollinearDistributionLHAPDF::computePolFF() {
    PartonDistribution partonDistribution;

    // Return empty distribution is the type does not match with the
    // function.
    if (m_type != CollinearDistributionType::PolFF)
        return partonDistribution;

    // Get distributions from LHAPDF (only positive values of x are allowed)
    const std::map<int, double> dists = m_set[m_member]->xfxQ2(std::abs(m_x),
            m_MuF2);

    // Map between QuarkFlavor enum and GPD convention for quarks
    const std::map<int, QuarkFlavor::Type> flavour_map {
            { 1, QuarkFlavor::DOWN }, { 2, QuarkFlavor::UP }, { 3,
                    QuarkFlavor::STRANGE }, { 4, QuarkFlavor::CHARM }, { 5,
                    QuarkFlavor::BOTTOM }, { 6, QuarkFlavor::TOP } };

    // Fill in map of QuarkDistribution
    std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributions;
    for (auto const& d : flavour_map) {

        // If the flavour isn't there continue
        if (dists.find(d.first) == dists.end()
                || dists.find(-d.first) == dists.end())
            continue;

        // Determine quark flavour
        const QuarkFlavor::Type type = flavour_map.at(std::abs(d.first));

        // Construct relevant combinations
        const double qrk = (m_x > 0 ? dists.at(d.first) : dists.at(-d.first));
        const double qrkPlus = (
                m_x > 0 ?
                        dists.at(d.first) - dists.at(-d.first) :
                        dists.at(-d.first) - dists.at(d.first));
        const double qrkMinus = (
                m_x > 0 ?
                        dists.at(d.first) + dists.at(-d.first) :
                        dists.at(-d.first) + dists.at(d.first));
        quarkDistributions.insert( { type, QuarkDistribution { type, qrk,
                qrkPlus, qrkMinus } });
    }

    // Put quark distributions into the PartonDistribution object
    partonDistribution.setQuarkDistributions(quarkDistributions);

    // Now set the gluon
    partonDistribution.setGluonDistribution(
            GluonDistribution { (m_x > 0 ? dists.at(21) : -dists.at(21)) });

    // Return
    return partonDistribution;
}

std::string CollinearDistributionLHAPDF::toString() const {
    return CollinearDistributionModule::toString();
}

std::string CollinearDistributionLHAPDF::getSetName() const {
    return m_setName;
}

int CollinearDistributionLHAPDF::getMember() const {
    return m_member;
}

CollinearDistributionType::Type CollinearDistributionLHAPDF::getType() const {
    return m_type;
}

std::vector<LHAPDF::PDF*> CollinearDistributionLHAPDF::getSet() const {
    return m_set;
}

void CollinearDistributionLHAPDF::setSetName(const std::string &setname) {
    m_setName = setname;
}

void CollinearDistributionLHAPDF::setMember(const int &member) {
    m_member = member;
}

void CollinearDistributionLHAPDF::setType(
        const CollinearDistributionType::Type &type) {

    m_listCollinearDistributionComputeTypeAvailable.clear();

    switch (type) {

    case CollinearDistributionType::UnpolPDF: {

        m_listCollinearDistributionComputeTypeAvailable.insert(
                std::make_pair(CollinearDistributionType::UnpolPDF,
                        &CollinearDistributionModule::computeUnpolPDF));
        break;
    }

    case CollinearDistributionType::PolPDF: {

        m_listCollinearDistributionComputeTypeAvailable.insert(
                std::make_pair(CollinearDistributionType::PolPDF,
                        &CollinearDistributionModule::computePolPDF));
        break;
    }

    case CollinearDistributionType::TransPDF: {

        m_listCollinearDistributionComputeTypeAvailable.insert(
                std::make_pair(CollinearDistributionType::TransPDF,
                        &CollinearDistributionModule::computeTransPDF));
        break;
    }

    case CollinearDistributionType::UnpolFF: {

        m_listCollinearDistributionComputeTypeAvailable.insert(
                std::make_pair(CollinearDistributionType::UnpolFF,
                        &CollinearDistributionModule::computeUnpolFF));
        break;
    }

    case CollinearDistributionType::PolFF: {

        m_listCollinearDistributionComputeTypeAvailable.insert(
                std::make_pair(CollinearDistributionType::PolFF,
                        &CollinearDistributionModule::computePolFF));
        break;
    }

    case CollinearDistributionType::TransFF: {

        m_listCollinearDistributionComputeTypeAvailable.insert(
                std::make_pair(CollinearDistributionType::TransFF,
                        &CollinearDistributionModule::computeTransFF));
        break;
    }

    default: {

        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Not able to relate function with type "
                        << CollinearDistributionType(type).toString());
        break;
    }
    }

    m_type = type;
}

} /* namespace PARTONS */
