#include "../../../../include/partons/modules/overlap/OverlapMMR15.h"

#include <cmath>
#include <map>
#include <utility>
#include <ElementaryUtils/logger/CustomException.h>
#include <NumA/functor/multi_dimension/FunctorMD.h>
#include <NumA/linear_algebra/vector/Vector2D.h>
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
            &OverlapMMR15::incompleteH);
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

double OverlapMMR15::incompleteH(NumA::VectorD& x_xi, std::vector<double>&) {
    if (x_xi.size() != 2)
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Array (x,xi) should be of two dimensions.");
    double x = x_xi[0];
    double xi = x_xi[1];
    if (x >= 0) {
        return 30 * (1 - x) * (1 - x) * (x * x - xi * xi)
                / ((1 - xi * xi) * (1 - xi * xi));
    } else {
        return 0.;
    }
}

PartonDistribution OverlapMMR15::computeH() {
    PartonDistribution partonDistribution;

    double Hu, Hd;

    if (isInKinematicRegion(m_x, m_xi)) {
        NumA::VectorD x_xi = NumA::Vector2D(m_x, m_xi);
        Hu = (*m_pIncompleteGPDFunction)(x_xi);
        x_xi[0] = -m_x;
        Hd = -(*m_pIncompleteGPDFunction)(x_xi);
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

