#include "../../../../include/partons/modules/overlap/OverlapMMR15.h"

#include <cmath>
#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int OverlapMMR15::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new OverlapMMR15("OverlapMMR15"));

//TODO initialise missing members
OverlapMMR15::OverlapMMR15(const std::string &className) :
        IncompleteGPDModule(className) {
    setKinematicRegion(DGLAP_REGION);

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &IncompleteGPDModule::computeH));
}

OverlapMMR15::OverlapMMR15(const OverlapMMR15& other) :
        IncompleteGPDModule(other) {
}

OverlapMMR15* OverlapMMR15::clone() const {
    return new OverlapMMR15(*this);
}

OverlapMMR15::~OverlapMMR15() {
}

void OverlapMMR15::configure(const ElemUtils::Parameters &parameters) {
    IncompleteGPDModule::configure(parameters);
}

//TODO implement
void OverlapMMR15::isModuleWellConfigured() {
    IncompleteGPDModule::isModuleWellConfigured();

}

bool OverlapMMR15::isInKinematicRegion(double x, double xi) {
    return (fabs(x) >= fabs(xi) and fabs(x) <= 1);
}

void OverlapMMR15::initModule() {
    IncompleteGPDModule::initModule();
}

PartonDistribution OverlapMMR15::computeH() {
    PartonDistribution partonDistribution;

    double H;

    if (m_x >= 0) {
        H = 30 * (1 - m_x) * (1 - m_x) * (m_x * m_x - m_xi * m_xi)
                / ((1 - m_xi * m_xi) * (1 - m_xi * m_xi));
    } else {
        H = 0.;
    }

    partonDistribution.addQuarkDistribution(
            QuarkDistribution(QuarkFlavor::UP, H));
    partonDistribution.addQuarkDistribution(
            QuarkDistribution(QuarkFlavor::DOWN, H));
    return partonDistribution;
}

std::string OverlapMMR15::toString() {
    return IncompleteGPDModule::toString();
}

