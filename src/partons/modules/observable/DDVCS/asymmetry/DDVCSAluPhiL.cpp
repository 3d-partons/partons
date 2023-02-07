#include "../../../../../../include/partons/modules/observable/DDVCS/asymmetry/DDVCSAluPhiL.h"

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

#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"

#include "../../../../../../include/partons/modules/observable/DDVCS/leptonCMframe.h"

namespace PARTONS {

const unsigned int DDVCSAluPhiL::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DDVCSAluPhiL("DDVCSAluPhiL"));

DDVCSAluPhiL::DDVCSAluPhiL(const std::string &className) :
        DDVCSObservable(className) {
}

DDVCSAluPhiL::DDVCSAluPhiL(const DDVCSAluPhiL& other) :
        DDVCSObservable(other) {
}

DDVCSAluPhiL::~DDVCSAluPhiL() {
}

DDVCSAluPhiL* DDVCSAluPhiL::clone() const {
    return new DDVCSAluPhiL(*this);
}

PhysicalType<double> DDVCSAluPhiL::computeObservable(
        const DDVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //parameters
    DDVCSAluPhiLParameters params;

    params.m_pDDVCSAluPhiL = this;
    params.m_xB = kinematic.getXB().getValue();
    params.m_t = kinematic.getT().getValue();
    params.m_Q2 = kinematic.getQ2().getValue();
    params.m_Q2Prim = kinematic.getQ2Prim().getValue();
    params.m_E = kinematic.getE().getValue();
    params.m_phiLBDP = kinematic.getPhiL().getValue();

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
    gsl_monte_function GAngleNum = { &DDVCSAluPhiLFunction, /*3*/
    2, &params };

    //range
    //    double min[] = { 0., 0., 0. };
    //    double max[] = { 2 * M_PI, M_PI,  2 * M_PI};
    double min[] = { 0., M_PI / 4. };
    double max[] = { 2 * M_PI, 3. * M_PI / 4. };

    //run
    gsl_monte_vegas_state* sAngleNum = gsl_monte_vegas_alloc(/*3*/2);

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngleNum, min, max, /*3*/2, 1000, rNum,
                sAngleNum, &num, &errNum);
    }

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
    gsl_monte_function GAngleDen = { &DDVCSAluPhiLFunction, /*3*/
    2, &params };

    //run
    gsl_monte_vegas_state* sAngleDen = gsl_monte_vegas_alloc(/*3*/2);

    for (size_t i = 0; i < 1; i++) {

        gsl_monte_vegas_integrate(&GAngleDen, min, max, /*3*/2, 1000, rDen,
                sAngleDen, &den, &errDen);
    }

    //free
    gsl_monte_vegas_free(sAngleDen);
    gsl_rng_free(rDen);

    //complete beam-spin asymmetry: num/den, dimensionless

    return PhysicalType<double>(num / den, PhysicalUnit::NONE);

}

double DDVCSAluPhiL::DDVCSAluPhiLFunction(double* kin, size_t dim, void* par) {

    double result;     //numerator or denominator of the asymmetry

    //parameters
    DDVCSAluPhiLParameters* params = static_cast<DDVCSAluPhiLParameters*>(par);

    //we want to integrate (following BDP2001 definition) over polar lepton angle and leave the asymmetry in terms of the azymuthal one
    leptons cmFrame;
    cmFrame.computeConverterVariables(params->m_xB, params->m_t, params->m_Q2,
            params->m_Q2Prim, Constant::PROTON_MASS);

    //lepton angles in BM2003 lepton-CM frame
    std::pair<double, double> cmFrameResult = cmFrame.leptonCMconverterToBM03(
            params->m_phiLBDP, kin[1]);
    double phiLBM03 = cmFrameResult.first;
    double thetaLBM03 = -1 * cmFrameResult.second;

//    std::cout << phiLBM03 << ", " << thetaLBM03 << ", " << params->m_phiLBDP << ", " << kin[1] << " phiLBM03, thetaLBM03, phiLBDP, thetaLBDP" << std::endl;//DEBUG

    //jacobian (jac)'s definition: d(xsec)/(... dthetaLBis dphiLBis) = (1/jac) * d(xsec)/(... dthetaLBM03 dphiLBM03)
    double jac = cmFrame.jacobianLeptonCM(phiLBM03, thetaLBM03);

//    std::cout << jac << " =jac before sin/sin" << std::endl;//DEBUG

    //observable uses lepton angles as in BM2003 lepton-CM frame
    DDVCSObservableKinematic ddvcsObservableKinematic(params->m_xB, params->m_t,
            params->m_Q2, params->m_Q2Prim, params->m_E, /*kin[2]*/
            kin[0], phiLBM03, thetaLBM03);


    //evaluate
    DDVCSObservableResult A =
            params->m_pDDVCSAluPhiL->getProcessModule()->compute(1., -1,
                    NumA::Vector3D(0., 0., 0.), ddvcsObservableKinematic,
                    params->m_gpdType, VCSSubProcessType::ALL);

    DDVCSObservableResult B =
            params->m_pDDVCSAluPhiL->getProcessModule()->compute(-1., -1,
                    NumA::Vector3D(0., 0., 0.), ddvcsObservableKinematic,
                    params->m_gpdType, VCSSubProcessType::ALL);

    if ((A.getValue().getValue() + B.getValue().getValue()) == 0.) {
        std::cout << "WARNING: Asymmetry denominator is zero" << std::endl;
        return 0.;
    }

    //numerator (signAux = -1) or denominator (signAux = +1) of asymmetry
    result = A.getValue().getValue()
            + (params->signAux) * B.getValue().getValue();

//    std::cout << result << " =A+-B, " << A.getValue().getValue() << " =A, " << B.getValue().getValue() << " =B" << std::endl;//DEBUG

    //DEBUG
    jac *= sin(kin[1])/sin(thetaLBM03);
//    std::cout << jac << " =jac, " << sin(kin[1]) << " =sin(thetaLBDP), " << sin(thetaLBM03) << " =sin(thetaLBM03) " << std::endl;
    //END DEBUG

    return result / jac;

}

} /* namespace PARTONS */
