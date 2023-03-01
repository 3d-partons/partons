#include "../../../../../../include/partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionUUMinusTCSLimit.h"

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DDVCS/DDVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSCrossSectionUUMinusTCSLimit::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCrossSectionUUMinusTCSLimit(
                        "DDVCSCrossSectionUUMinusTCSLimit"));

DDVCSCrossSectionUUMinusTCSLimit::DDVCSCrossSectionUUMinusTCSLimit(
        const std::string &className) :
        DDVCSObservable(className) {
}

DDVCSCrossSectionUUMinusTCSLimit::DDVCSCrossSectionUUMinusTCSLimit(
        const DDVCSCrossSectionUUMinusTCSLimit& other) :
        DDVCSObservable(other) {
}

DDVCSCrossSectionUUMinusTCSLimit::~DDVCSCrossSectionUUMinusTCSLimit() {
}

DDVCSCrossSectionUUMinusTCSLimit* DDVCSCrossSectionUUMinusTCSLimit::clone() const {
    return new DDVCSCrossSectionUUMinusTCSLimit(*this);
}

PhysicalType<double> DDVCSCrossSectionUUMinusTCSLimit::computeObservable(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //result and error
    double res, err;

    //parameters
    DDVCSCrossSectionUUMinusTCSLimitParameters params;

    params.m_pDDVCSCrossSectionUUMinusTCSLimit = this;
    params.m_xB = kinematic.getXB().getValue();
    params.m_t = kinematic.getT().getValue();
    params.m_Q2 = kinematic.getQ2().getValue();
    params.m_Q2Prim = kinematic.getQ2Prim().getValue();
    params.m_E = kinematic.getE().getValue();
    params.m_phiL = kinematic.getPhiL().getValue();
    params.m_thetaL = kinematic.getThetaL().getValue();

    params.m_gpdType = gpdType;

    //random
    gsl_rng* r = gsl_rng_alloc(gsl_rng_default);

    //function
    gsl_monte_function GAngle = { &DDVCSCrossSectionUUMinusTCSLimitFunction, 1,
            &params };

    //range
    double min[] = { 0. };
    double max[] = { 2. * M_PI };

    //run
    gsl_monte_vegas_state* sAngle = gsl_monte_vegas_alloc(1);

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngle, min, max, 1, 10000, r, sAngle, &res,
                &err);
    }

    //free
    gsl_monte_vegas_free(sAngle);
    gsl_rng_free(r);

    //change to nb
    return PhysicalType<double>(res, PhysicalUnit::NB);
}

double DDVCSCrossSectionUUMinusTCSLimit::DDVCSCrossSectionUUMinusTCSLimitFunction(
        double* kin, size_t dim, void* par) {

    //parameters
    DDVCSCrossSectionUUMinusTCSLimitParameters* params =
            static_cast<DDVCSCrossSectionUUMinusTCSLimitParameters*>(par);

    //observable
    DDVCSObservableKinematic ddvcsObservableKinematic(params->m_xB, params->m_t,
            params->m_Q2, params->m_Q2Prim, params->m_E, kin[0], params->m_phiL,
            params->m_thetaL);

    //evaluate
    DDVCSObservableResult A =
            params->m_pDDVCSCrossSectionUUMinusTCSLimit->getProcessModule()->compute(
                    1., -1, NumA::Vector3D(0., 0., 0.),
                    ddvcsObservableKinematic, params->m_gpdType,
                    VCSSubProcessType::INT);

    DDVCSObservableResult B =
            params->m_pDDVCSCrossSectionUUMinusTCSLimit->getProcessModule()->compute(
                    -1., -1, NumA::Vector3D(0., 0., 0.),
                    ddvcsObservableKinematic, params->m_gpdType,
                    VCSSubProcessType::INT);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

//    result = A.getValue();//DEBUG

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB).getValue();

}

} /* namespace PARTONS */
