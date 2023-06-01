#include "../../../../../../include/partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionUUMinusDVCSLimit.h"

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

const unsigned int DDVCSCrossSectionUUMinusDVCSLimit::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSCrossSectionUUMinusDVCSLimit(
                        "DDVCSCrossSectionUUMinusDVCSLimit"));

DDVCSCrossSectionUUMinusDVCSLimit::DDVCSCrossSectionUUMinusDVCSLimit(
        const std::string &className) :
        DDVCSObservable(className) {
}

DDVCSCrossSectionUUMinusDVCSLimit::DDVCSCrossSectionUUMinusDVCSLimit(
        const DDVCSCrossSectionUUMinusDVCSLimit& other) :
        DDVCSObservable(other) {
}

DDVCSCrossSectionUUMinusDVCSLimit::~DDVCSCrossSectionUUMinusDVCSLimit() {
}

DDVCSCrossSectionUUMinusDVCSLimit* DDVCSCrossSectionUUMinusDVCSLimit::clone() const {
    return new DDVCSCrossSectionUUMinusDVCSLimit(*this);
}

PhysicalType<double> DDVCSCrossSectionUUMinusDVCSLimit::computeObservable(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //result and error
    double res, err;

    //parameters
    DDVCSCrossSectionUUMinusDVCSLimitParameters params;

    params.m_pDDVCSCrossSectionUUMinusDVCSLimit = this;
    params.m_xB = kinematic.getXB().getValue();
    params.m_t = kinematic.getT().getValue();
    params.m_Q2 = kinematic.getQ2().getValue();
    params.m_Q2Prim = kinematic.getQ2Prim().getValue();
    params.m_E = kinematic.getE().getValue();
    params.m_phi = kinematic.getPhi().getValue();

    params.m_gpdType = gpdType;

    //random
    gsl_rng* r = gsl_rng_alloc(gsl_rng_default);

    //function
    gsl_monte_function GAngle = { &DDVCSCrossSectionUUMinusDVCSLimitFunction, 2,
            &params };

    //range
//    double min[] = { 0., 0., 0. };
//    double max[] = { 2 * M_PI, M_PI,  2 * M_PI};
    double min[] = { 0., 0. };
    double max[] = { 2 * M_PI, M_PI };

    //run
    gsl_monte_vegas_state* sAngle = gsl_monte_vegas_alloc(2);

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngle, min, max, 2, 4000, r, sAngle, &res,
                &err);
    }

    std::cout << PhysicalType<double>(res, PhysicalUnit::NB).toString() << " "
            << PhysicalType<double>(err, PhysicalUnit::NB).toString()
            << " res err" << std::endl;

    //free
    gsl_monte_vegas_free(sAngle);
    gsl_rng_free(r);

    //change to nb
    return PhysicalType<double>(res, PhysicalUnit::NB);
}

double DDVCSCrossSectionUUMinusDVCSLimit::DDVCSCrossSectionUUMinusDVCSLimitFunction(
        double* kin, size_t dim, void* par) {

    //parameters
    DDVCSCrossSectionUUMinusDVCSLimitParameters* params =
            static_cast<DDVCSCrossSectionUUMinusDVCSLimitParameters*>(par);

    //observable
    DDVCSObservableKinematic ddvcsObservableKinematic(params->m_xB, params->m_t,
            params->m_Q2, params->m_Q2Prim, params->m_E, params->m_phi, kin[0],
            kin[1]);

    //evaluate
    DDVCSObservableResult A =
            params->m_pDDVCSCrossSectionUUMinusDVCSLimit->getProcessModule()->compute(
                    1., -1, NumA::Vector3D(0., 0., 0.),
                    ddvcsObservableKinematic, params->m_gpdType,
                    VCSSubProcessType::DDVCS);

    DDVCSObservableResult B =
            params->m_pDDVCSCrossSectionUUMinusDVCSLimit->getProcessModule()->compute(
                    -1., -1, NumA::Vector3D(0., 0., 0.),
                    ddvcsObservableKinematic, params->m_gpdType,
                    VCSSubProcessType::DDVCS);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB).getValue();

}

} /* namespace PARTONS */
