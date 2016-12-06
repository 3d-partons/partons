#include "../../../../include/partons/modules/overlap/OverlapMMR15.h"

#include <cmath>
#include <map>
#include <utility>
//#include <vector>
#include <ElementaryUtils/logger/CustomException.h>
#include <NumA/functor/multi_dimension/FunctorMD.h>
#include <NumA/utils/FunctorUtils.h>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/radon_inverse/RadonInverseModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int OverlapMMR15::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new OverlapMMR15("OverlapMMR15"));

//TODO initialise missing members
OverlapMMR15::OverlapMMR15(const std::string &className) :
        IncompleteGPDModule(className) {
    setInversionDependent(true);
    m_pIncompleteGPDFunction = NumA::FunctorUtils::newFunctorMD(this,
            &OverlapMMR15::functorH);
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

void OverlapMMR15::resolveObjectDependencies() {
    // Cause of random static variable resolution
    // Kinematic region must be set after the resolution of all static variables
    // Cannot be done into the constructor.
    setKinematicRegion(IncompleteGPDModule::DGLAP_REGION);
}

void OverlapMMR15::initModule() {
    IncompleteGPDModule::initModule();
}

double OverlapMMR15::incompleteH(double x, double xi) {
    if (x >= 0) {
        double xi2 = xi * xi;
        double x1 = 1 - x;
        double xi21 = 1 - xi2;
        return 30 * x1 * x1 * (x * x - xi2) / (xi21 * xi21);
    } else {
        return 0.;
    }
}

double OverlapMMR15::functorH(NumA::VectorD& x_xi, std::vector<double>&) {
    if (x_xi.size() != 2)
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Array (x,xi) should be of two dimensions.");
    double x = x_xi[0];
    double xi = x_xi[1];
    return incompleteH(x, xi);
}

PartonDistribution OverlapMMR15::computeH() {
    PartonDistribution partonDistribution;

    double Hu, Hd;

    if (isInKinematicRegion(m_x, m_xi)) {
        Hu = incompleteH(m_x, m_xi);
        Hd = -incompleteH(-m_x, m_xi);
    } else {
        //TODO Radon Inverse
        Hu = m_pRadonInverse->computeGPD(m_x, m_xi);
        Hd = -m_pRadonInverse->computeGPD(-m_x, m_xi);
    }

    partonDistribution.addQuarkDistribution(
            QuarkDistribution(QuarkFlavor::UP, Hu, Hu + Hd, Hu - Hd));
    partonDistribution.addQuarkDistribution(
            QuarkDistribution(QuarkFlavor::DOWN, Hd, Hd + Hu, Hd - Hu));
    partonDistribution.addQuarkDistribution(
            QuarkDistribution(QuarkFlavor::STRANGE, 0., 0., 0.));
    return partonDistribution;
}

std::string OverlapMMR15::toString() {
    return IncompleteGPDModule::toString();
}
