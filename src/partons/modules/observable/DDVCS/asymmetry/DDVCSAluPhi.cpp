#include "../../../../../../include/partons/modules/observable/DDVCS/asymmetry/DDVCSAluPhi.h"

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>

#include "../../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DDVCS/DDVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DDVCSAluPhi::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSAluPhi("DDVCSAluPhi"));

DDVCSAluPhi::DDVCSAluPhi(const std::string &className) :
        DDVCSObservable(className) {
}

DDVCSAluPhi::DDVCSAluPhi(const DDVCSAluPhi& other) :
        DDVCSObservable(other) {
}

DDVCSAluPhi::~DDVCSAluPhi() {
}

DDVCSAluPhi* DDVCSAluPhi::clone() const {
    return new DDVCSAluPhi(*this);
}

PhysicalType<double> DDVCSAluPhi::computeObservable(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //parameters
    DDVCSAluPhiParameters params;

    params.m_pDDVCSAluPhi = this;
    params.m_xB = kinematic.getXB().getValue();
    params.m_t = kinematic.getT().getValue();
    params.m_Q2 = kinematic.getQ2().getValue();
    params.m_Q2Prim = kinematic.getQ2Prim().getValue();
    params.m_E = kinematic.getE().getValue();
    params.m_phi = kinematic.getPhi().getValue();

    params.m_gpdType = gpdType;

    // *******************
    // **** compute numerator of asymmetry
    // *******************

    //numerator and its error
    double num, errNum;

    params.signAux = -1.;

    //random
    gsl_rng* rNum = gsl_rng_alloc(gsl_rng_default);

    //function
    gsl_monte_function GAngleNum = { &DDVCSAluPhiFunction, /*3*/
    2, &params };

    //range
    //    double min[] = { 0., 0., 0. };
    //    double max[] = { 2 * M_PI, M_PI,  2 * M_PI};
    double min[] = { 0., M_PI / 4. };
    double max[] = { 2 * M_PI, 3. * M_PI / 4. };

    //run
    gsl_monte_vegas_state* sAngleNum = gsl_monte_vegas_alloc(/*3*/2);

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngleNum, min, max, /*3*/2, 10000, rNum,
                sAngleNum, &num, &errNum);
    }

    std::cout << PhysicalType<double>(num, PhysicalUnit::NB).toString() << " "
                    << PhysicalType<double>(errNum, PhysicalUnit::NB).toString()
                    << " num errNum" << std::endl;

    //free
    gsl_monte_vegas_free(sAngleNum);
    gsl_rng_free(rNum);

    // *******************
    // **** compute denominator of asymmetry
    // *******************

    //denominator and its error
    double den, errDen;

    params.signAux = +1.;

    //random
    gsl_rng* rDen = gsl_rng_alloc(gsl_rng_default);

    //function
    gsl_monte_function GAngleDen = { &DDVCSAluPhiFunction, /*3*/
    2, &params };

    //run
    gsl_monte_vegas_state* sAngleDen = gsl_monte_vegas_alloc(/*3*/2);

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngleDen, min, max, /*3*/2, 4000, rDen,
                sAngleDen, &den, &errDen);
    }

    std::cout << PhysicalType<double>(den, PhysicalUnit::NB).toString() << " "
                    << PhysicalType<double>(errDen, PhysicalUnit::NB).toString()
                    << " den errDen" << std::endl;

    //free
    gsl_monte_vegas_free(sAngleDen);
    gsl_rng_free(rDen);

    //complete spin beam asymmetry: num/den, dimensionless

    return PhysicalType<double>(num / den, PhysicalUnit::NONE);

}

double DDVCSAluPhi::DDVCSAluPhiFunction(double* kin, size_t dim, void* par) {

    double result;     //numerator or denominator of the asymmetry

    //parameters
    DDVCSAluPhiParameters* params = static_cast<DDVCSAluPhiParameters*>(par);

    //observable
    DDVCSObservableKinematic ddvcsObservableKinematic(params->m_xB, params->m_t,
            params->m_Q2, params->m_Q2Prim, params->m_E, params->m_phi, kin[0],
            kin[1]);

    //evaluate
    DDVCSObservableResult A =
            params->m_pDDVCSAluPhi->getProcessModule()->compute(1., -1,
                    NumA::Vector3D(0., 0., 0.), ddvcsObservableKinematic,
                    params->m_gpdType, VCSSubProcessType::ALL);

    DDVCSObservableResult B =
            params->m_pDDVCSAluPhi->getProcessModule()->compute(-1., -1,
                    NumA::Vector3D(0., 0., 0.), ddvcsObservableKinematic,
                    params->m_gpdType, VCSSubProcessType::ALL);

    if ((A.getValue().getValue() + B.getValue().getValue()) == 0.) {
        std::cout << "WARNING: Asymmetry denominator is zero" << std::endl;
        return 0.;
    }

    //numerator (signAux = -1) or denominator (signAux = +1) of asymmetry
    result = A.getValue().getValue()
            + (params->signAux) * B.getValue().getValue();

    return result;

}

} /* namespace PARTONS */
