#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSCFFHeavyQuarkModel.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <cmath>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/GPDModule.h"

// Initialise [class]::classId with a unique name.
const unsigned int DVCSCFFHeavyQuarkModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFHeavyQuarkModel("DVCSCFFHeavyQuarkModel"));

DVCSCFFHeavyQuarkModel::DVCSCFFHeavyQuarkModel(const std::string& className) :
        DVCSCFFModel(className), m_betas(std::complex<double>(0.0, 0.0)), m_rs(
                std::complex<double>(0.0, 0.0)), m_betaq(
                std::complex<double>(0.0, 0.0)), m_rq(
                std::complex<double>(0.0, 0.0)), m_TF(
                std::complex<double>(0.5, 0.0)) {

    initFunctorsForIntegrations();
}

DVCSCFFHeavyQuarkModel* DVCSCFFHeavyQuarkModel::clone() const {
    return new DVCSCFFHeavyQuarkModel(*this);
}

DVCSCFFHeavyQuarkModel::~DVCSCFFHeavyQuarkModel() {
    if (m_pConvolReKernelGluonMassiveV) {
        delete m_pConvolReKernelGluonMassiveV;
        m_pConvolReKernelGluonMassiveV = 0;
    }

    if (m_pConvolImKernelGluonMassiveV) {
        delete m_pConvolImKernelGluonMassiveV;
        m_pConvolImKernelGluonMassiveV = 0;
    }

    if (m_pConvolReKernelGluonMassiveA) {
        delete m_pConvolReKernelGluonMassiveA;
        m_pConvolReKernelGluonMassiveA = 0;
    }

    if (m_pConvolImKernelGluonMassiveA) {
        delete m_pConvolImKernelGluonMassiveA;
        m_pConvolImKernelGluonMassiveA = 0;
    }
}

void DVCSCFFHeavyQuarkModel::resolveObjectDependencies() {
    DVCSConvolCoeffFunctionModule::resolveObjectDependencies();

    setIntegrator(NumA::IntegratorType1D::DEXP);
}

void DVCSCFFHeavyQuarkModel::initFunctorsForIntegrations() {
    m_pConvolReKernelGluonMassiveV = NumA::Integrator1D::newIntegrationFunctor(
            this, &DVCSCFFHeavyQuarkModel::ConvolReKernelGluonMassiveV);
    m_pConvolImKernelGluonMassiveV = NumA::Integrator1D::newIntegrationFunctor(
            this, &DVCSCFFHeavyQuarkModel::ConvolImKernelGluonMassiveV);
    m_pConvolReKernelGluonMassiveA = NumA::Integrator1D::newIntegrationFunctor(
            this, &DVCSCFFHeavyQuarkModel::ConvolReKernelGluonMassiveA);
    m_pConvolImKernelGluonMassiveA = NumA::Integrator1D::newIntegrationFunctor(
            this, &DVCSCFFHeavyQuarkModel::ConvolImKernelGluonMassiveA);
}

DVCSCFFHeavyQuarkModel::DVCSCFFHeavyQuarkModel(
        const DVCSCFFHeavyQuarkModel& other) :
        DVCSCFFModel(other) {
    m_betas = other.m_betas;
    m_betaq = other.m_betaq;
    m_rs = other.m_rs;
    m_rq = other.m_rq;
    m_TF = other.m_TF;

    initFunctorsForIntegrations();
}

std::complex<double> DVCSCFFHeavyQuarkModel::computeUnpolarized() {

    return computeIntegralsMassiveV() + DVCSCFFModel::computeUnpolarized();
//    return computeIntegralsMassiveV();
}

std::complex<double> DVCSCFFHeavyQuarkModel::computePolarized() {

    return computeIntegralsMassiveA() + DVCSCFFModel::computePolarized();
//  return computeIntegralsMassiveA();
}

std::complex<double> DVCSCFFHeavyQuarkModel::computeIntegralsMassiveV() {
    double IntegralImaginaryPartMassive = 0.;
    double IntegralRealPartMassive = 0.;

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {

        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    // Compute sum of active quark electric charges squared

    //  Integrator.SetAbsTolerance( 0. );
    //   Integrator.SetRelTolerance( 0.001 );
    //   Integrator.SetNPoints( 1 );

    // Gluon sector

    std::vector<double> emptyParameters;

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        IntegralRealPartMassive = integrate(m_pConvolReKernelGluonMassiveV, 0.,
                +1., emptyParameters);

        IntegralImaginaryPartMassive = integrate(m_pConvolImKernelGluonMassiveV,
                0., +1., emptyParameters);
    }

    return std::complex<double>(IntegralRealPartMassive,
            IntegralImaginaryPartMassive);

}

std::complex<double> DVCSCFFHeavyQuarkModel::computeIntegralsMassiveA() {
    double IntegralImaginaryPartMassive = 0.;
    double IntegralRealPartMassive = 0.;

    if (m_qcdOrderType != PerturbativeQCDOrderType::LO
            && m_qcdOrderType != PerturbativeQCDOrderType::NLO) {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Erroneous input perturbative QCD order can only be LO or NLO. Here Order = "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString());
    }

    // Compute sum of active quark electric charges squared

    //  Integrator.SetAbsTolerance( 0. );
    //   Integrator.SetRelTolerance( 0.001 );
    //   Integrator.SetNPoints( 1 );

    // Gluon sector

    std::vector<double> emptyParameters;

    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {
        IntegralRealPartMassive = integrate(m_pConvolReKernelGluonMassiveA, 0.,
                +1., emptyParameters);

        IntegralImaginaryPartMassive = integrate(m_pConvolImKernelGluonMassiveA,
                0., +1., emptyParameters);
    }

    return std::complex<double>(IntegralRealPartMassive,
            IntegralImaginaryPartMassive);
}

std::complex<double> DVCSCFFHeavyQuarkModel::beta(double s, double mq) {
    std::complex<double> tmp_beta;
    if (s < 0) {
        tmp_beta.real(std::sqrt(1. - 4. * mq * mq / s));
        tmp_beta.imag(0.);
    }
    if ((s > 0) && (s < 4. * mq * mq)) {
        tmp_beta.real(0.);
        tmp_beta.imag(std::sqrt(-1. + 4. * mq * mq / s));
    }
    if (s >= 4 * mq * mq) {
        tmp_beta.real(std::sqrt(1. - 4. * mq * mq / s));
        tmp_beta.imag(0.);
    }
    return tmp_beta;
}

std::complex<double> DVCSCFFHeavyQuarkModel::r(double s, double mq) {
    std::complex<double> tmp_beta = beta(s, mq);
    return (tmp_beta - 1.) / (tmp_beta + 1.);
}

double DVCSCFFHeavyQuarkModel::ConvolReKernelGluonMassiveV(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(x, m_xi, m_t,
            m_MuF2, m_MuR2, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD) * MassiveKernelGluonNLOV(x).real();
    return Convol;
}

double DVCSCFFHeavyQuarkModel::ConvolImKernelGluonMassiveV(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(x, m_xi, m_t,
            m_MuF2, m_MuR2, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD) * MassiveKernelGluonNLOV(x).imag();
    return Convol;
}

double DVCSCFFHeavyQuarkModel::ConvolReKernelGluonMassiveA(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(x, m_xi, m_t,
            m_MuF2, m_MuR2, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD) * MassiveKernelGluonNLOA(x).real();
    return Convol;
}

double DVCSCFFHeavyQuarkModel::ConvolImKernelGluonMassiveA(double x,
        std::vector<double> params) {
    PartonDistribution partonDistribution = m_pGPDModule->compute(x, m_xi, m_t,
            m_MuF2, m_MuR2, m_currentGPDComputeType);

    // GPD evaluated at x = x[ 0 ]
    double EvalGPD = 2
            * partonDistribution.getGluonDistribution().getGluonDistribution();

    double Convol = (EvalGPD) * MassiveKernelGluonNLOA(x).imag();
    return Convol;
}

std::complex<double> DVCSCFFHeavyQuarkModel::MassiveKernelGluonNLOA(double x) {
//    m_pLoggerManager->debug(getClassName(), __func__, "entered");
// TODO Add a massless part
    double z = m_xi / x;
    double mq = QUARK_CHARM_MASS;
    double s = -m_Q2 * (z - 1.) / 2. / z;
    double SumSqrCharges; // Sum of square of electric charges of active quark flavours

    m_betas = beta(s, mq);
    m_rs = r(s, mq);
    m_betaq = beta(-m_Q2, mq);
    m_rq = r(-m_Q2, mq);
    std::complex<double> GluonNLOA(0.0, 0.0);
    std::complex<double> GluonNLOAa(0.0, 0.0);
    std::complex<double> GluonNLOAb(0.0, 0.0);

    if (z == 1.) {
        GluonNLOA.real(1.0);
        GluonNLOA.imag(0.);
    }
    if (z == -1.) {
        GluonNLOA.real(-1.0);
        GluonNLOA.imag(0.);
    }

    ///////////////// c(z)

    if ((z != 1) && (z != -1)) {
        GluonNLOAa = m_betas * std::log(m_rs) - m_betaq * std::log(m_rq);
        GluonNLOAa *= (2. * z - 6.) / (z - 1.);
        GluonNLOAa += std::log(m_rs) * std::log(m_rs)
                - std::log(m_rq) * std::log(m_rq);
        GluonNLOAa *= (m_TF / 2. / (z + 1.) / (z + 1.));
    }

///////////////// c(-z)
    z = -z;
    s = -m_Q2 * (z - 1.) / 2. / z;
    m_betas = beta(s, mq);
    m_rs = r(s, mq);
    if ((z != 1) && (z != -1)) {
        GluonNLOAb = m_betas * std::log(m_rs) - m_betaq * std::log(m_rq);
        GluonNLOAb *= (2. * z - 6.) / (z - 1.);
        GluonNLOAb += std::log(m_rs) * std::log(m_rs)
                - std::log(m_rq) * std::log(m_rq);
        GluonNLOAb *= (m_TF / 2. / (z + 1.) / (z + 1.));
    }

    GluonNLOA = m_alphaSOver2Pi * C_ELEC_CHARGE * C_ELEC_CHARGE
            * (GluonNLOAa - GluonNLOAb);
    ;
    GluonNLOA /= x * x;

    debug(__func__,
            ElemUtils::Formatter() << "x= " << x << "    GluonNLOA RE = "
                    << GluonNLOA.real() << "   GluonNLOA IM = "
                    << GluonNLOA.imag() << "  alphaSover2Pi = "
                    << m_alphaSOver2Pi);

    return GluonNLOA;
}

std::complex<double> DVCSCFFHeavyQuarkModel::MassiveKernelGluonNLOV(double x) {
    double z = -m_xi / x;
    double mq = QUARK_CHARM_MASS;
    double s = -m_Q2 * (z - 1.) / 2. / z;
    double eta = mq * mq / m_Q2;
    double SumSqrCharges; // Sum of square of electric charges of active quark flavours
    std::complex<double> GluonNLOV(0.0, 0.0);
    std::complex<double> GluonNLOVa(0.0, 0.0);
    std::complex<double> GluonNLOVb(0.0, 0.0);
    if (m_qcdOrderType == PerturbativeQCDOrderType::NLO) {

        m_betas = beta(s, mq);
        m_rs = r(s, mq);
        m_betaq = beta(-m_Q2, mq);
        m_rq = r(-m_Q2, mq);

        if (z == 1.) {
            GluonNLOV.real(0.);
            GluonNLOV.imag(0.);
        }
        if (z == -1.) {
            GluonNLOV.real(0.);
            GluonNLOV.imag(0.);
        }
        if ((z != 1.) && (z != -1.)) {
////////////////// c(z)
            GluonNLOVa = m_betas * std::log(m_rs) - m_betaq * std::log(m_rq);
            GluonNLOVa *= (2. - z * (6. - 16. * eta)) / (z - 1.);
            GluonNLOVa -= (1.
                    + 8. * eta * z * (1. - (1. + 4. * eta) * z) / (z - 1.)
                            / (z - 1.)) * std::log(m_rs) * std::log(m_rs);
            GluonNLOVa += (1.
                    - 2. * eta * (1. + (3. + 8. * eta) * z * z) / (z - 1.)
                            / (z - 1.)) * std::log(m_rq) * std::log(m_rq);
            GluonNLOVa *= (m_TF / 2. / (z + 1.) / (z + 1.));
///////////////// c(-z)
            z = -z;
            s = -m_Q2 * (z - 1.) / 2. / z;
            m_betas = beta(s, mq);
            m_rs = r(s, mq);

            GluonNLOVb = m_betas * std::log(m_rs) - m_betaq * std::log(m_rq);
            GluonNLOVb *= (2. - z * (6. - 16. * eta)) / (z - 1.);
            GluonNLOVb -= (1.
                    + 8. * eta * z * (1. - (1. + 4. * eta) * z) / (z - 1.)
                            / (z - 1.)) * std::log(m_rs) * std::log(m_rs);
            GluonNLOVb += (1.
                    - 2. * eta * (1. + (3. + 8. * eta) * z * z) / (z - 1.)
                            / (z - 1.)) * std::log(m_rq) * std::log(m_rq);
            GluonNLOVb *= (m_TF / 2. / (z + 1.) / (z + 1.));

        }

        GluonNLOV = m_alphaSOver2Pi * C_ELEC_CHARGE * C_ELEC_CHARGE
                * (GluonNLOVa + GluonNLOVb);
        GluonNLOV /= x * x;

    }

    debug(__func__, ElemUtils::Formatter() << "x= " << x
//            << "    GluonNLOV RE = " << GluonNLOV.real()
//            << "   GluonNLOV IM = "<< GluonNLOV.imag()
            << "  alphaSover2Pi = " << m_alphaSOver2Pi);

    return GluonNLOV;
}
