#include "../../../../include/partons/modules/gpd/MPSSW13WithDTermModel.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <cmath>
#include <map>
#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/utils/MSTWPDF.h"
#include "../../../../include/partons/utils/PartonContent.h"

// Initialise [class]::classId with a unique name.
const unsigned int MPSSW13WithDTermModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new MPSSW13WithDTermModel("MPSSW13WithDTermModel"));

MPSSW13WithDTermModel::MPSSW13WithDTermModel(const std::string &className) :
        GPDModule(className), MathIntegratorModule(), m_Forward(0), m_NbOfQuarkFlavor(
                2), m_NbOfColor(3), m_Mx(0.), m_CA(3.), m_CF(4. / 3.), m_TF(
                1. / 2.), m_F1u(0.), m_F1d(0.), m_FD(0.), m_ProfileShapeVal(1.), m_ProfileShapeSea(
                2.), m_ProfileShapeGlue(2.), m_QuarkDTerm(0.), m_GluonDTerm(0.) {

    m_NbOfQuarkFlavor = 3;
    m_MuF2 = 4.;
    m_MuF2_ref = 4.;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    initFunctorsForIntegrations();

}

MPSSW13WithDTermModel::MPSSW13WithDTermModel(const MPSSW13WithDTermModel& other) :
        GPDModule(other), MathIntegratorModule(other) {
    m_NbOfQuarkFlavor = other.m_NbOfQuarkFlavor;
    m_NbOfColor = other.m_NbOfColor;
    m_Mx = other.m_Mx;
    m_CA = other.m_CA;
    m_CF = other.m_CF;
    m_TF = other.m_TF;
    m_F1u = other.m_F1u;
    m_F1d = other.m_F1d;
    m_FD = other.m_FD;
    m_ProfileShapeVal = other.m_ProfileShapeVal;
    m_ProfileShapeSea = other.m_ProfileShapeSea;
    m_ProfileShapeGlue = other.m_ProfileShapeGlue;
    m_QuarkDTerm = other.m_QuarkDTerm;
    m_GluonDTerm = other.m_GluonDTerm;

    //TODO make a clone instance ; create MSTWPDF as a module.
    m_Forward = new MSTWPDF();
    m_Forward->init(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "grid.directory") + "mstw2008nlo.00.dat");

    initFunctorsForIntegrations();
}

MPSSW13WithDTermModel::~MPSSW13WithDTermModel() {
    if (m_Forward) {
        delete m_Forward;
        m_Forward = 0;
    }
}

void MPSSW13WithDTermModel::initFunctorsForIntegrations() {
    m_pIntegralHuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralHuVal);

    m_pIntegralHdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralHdVal);

    m_pIntegralHuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralHuValMx);

    m_pIntegralHdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralHdValMx);

    m_pIntegralxLargeHuSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHuSea);

    m_pIntegralxLargeHdSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHdSea);

    m_pIntegralxSmall1HuSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall1HuSea);

    m_pIntegralxSmall2HuSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall2HuSea);

    m_pIntegralxSmall1HdSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall1HdSea);

    m_pIntegralxSmall2HdSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall2HdSea);

    m_pIntegralxLargeHuSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHuSeaMx);

    m_pIntegralxLargeHdSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHdSeaMx);

    m_pIntegralxLargeHsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHsSea);

    m_pIntegralxSmall1HsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall1HsSea);

    m_pIntegralxSmall2HsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall2HsSea);

    m_pIntegralxLargeHsSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHsSeaMx);

    m_pIntegralxLargeHg = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHg);

    m_pIntegralxSmall1Hg = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall1Hg);

    m_pIntegralxSmall2Hg = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxSmall2Hg);

    m_pIntegralxLargeHgMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &MPSSW13WithDTermModel::IntegralxLargeHgMx);
}

void MPSSW13WithDTermModel::resolveObjectDependencies() {
    setIntegrator(NumA::IntegratorType1D::DEXP);
}

MPSSW13WithDTermModel* MPSSW13WithDTermModel::clone() const {
    return new MPSSW13WithDTermModel(*this);
}

void MPSSW13WithDTermModel::setParameters(std::vector<double> Parameters) {
    // TODO: Check general syntax of setParameters...

    if (Parameters.size() != 3) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Missing parameters !");
    }

    // Test known constraints on free parameters
    if (Parameters.at(0) < 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Exponent in valence profile function should be >= 1."
                        << '\n' << "Here exponent = " << Parameters.at(0)
                        << '\n');
    }
    if (Parameters.at(1) < 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Exponent in sea profile function should be >= 1."
                        << '\n' << "Here exponent = " << Parameters.at(1)
                        << '\n');
    }
    if (Parameters.at(2) < 1.) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Exponent in glue profile function should be >= 1."
                        << '\n' << "Here exponent = " << Parameters.at(2)
                        << '\n');
    }

    // Assign parameters
    m_ProfileShapeVal = Parameters.at(0);
    m_ProfileShapeSea = Parameters.at(1);
    m_ProfileShapeGlue = Parameters.at(2);
}

void MPSSW13WithDTermModel::ComputeDTerms(double x) {
    //TODO divided by zero !
    double z = x / m_xi;

    const double Mu0 = 0.6; // Reference scale (in GeV)
    const double CA2 = m_CA * m_CA;
    const double Lambda3 = 0.232; // LambdaQCD with 3 active flavours (in GeV)
    const double Lambda4 = 0.200; // LambdaQCD with 4 active flavours (in GeV)
    const double MuSwitch = 1.5; // NFlavour = 3 if fMuR < MuSwitch, 4 otherwise (in GeV)

    double Beta0[2]; // 1st coefficient of QCD Beta function
    double Beta1[2]; // 2nd coefficient of QCD Beta function
    double AlphaS0; // Strong running coupling at reference scale
    double AlphaSMuSwitch; // Strong running coupling at charm threshold MuSwitch
    double AlphaS; // Strong running coupling at current scale
    double Lambda; // Current value of LambdaQCD, either Lambda3 or Lambda4
    double t; // t = 2 log( t / LambdaQCD )
    double lt; // lt = log t
    unsigned int NFlavour;

    const unsigned int nGegen = 3;
    double QuarkGegenbauer[nGegen]; // Gegenbauer polynomials C3/2
    double GluonGegenbauer[nGegen]; // Gegenbauer polynomials C5/2
    double TabGammaQQ[2][nGegen]; // Anomalous dimension eq. (42)
    double TabGammaQG[2][nGegen]; // Anomalous dimension eq. (42)
    double TabGammaGQ[2][nGegen]; // Anomalous dimension eq. (42)
    double TabGammaGG[2][nGegen]; // Anomalous dimension eq. (42)

    double TabGammaPrimeQG[2][nGegen]; // Anomalous dimension eq. (45)
    double TabGammaPrimeGQ[2][nGegen]; // Anomalous dimension eq. (45)

    double CapitalGammap[2][nGegen]; // Anomalous dimension eq. (46)
    double CapitalGammam[2][nGegen]; // Anomalous dimension eq. (46)

    double gp[2][nGegen]; // Eigenvector eq. (48)
    double gm[2][nGegen]; // Eigenvector eq. (48)

    double Ap[2][nGegen]; // Coefficients for scale-dependence in eq. (51)
    double Am[2][nGegen]; // Coefficients for scale-dependence in eq. (51)

    double Kp[2][nGegen]; // Anomalous dimension between eq. (51) and eq. (52)
    double Km[2][nGegen]; // Anomalous dimension between eq. (51) and eq. (52)

    double Quarkd[nGegen]; // Coefficients of the D-term expansion over Gegenbauer polynomial basis C3/2 at current scale
    double Gluond[nGegen]; // Coefficients of the D-term expansion over Gegenbauer polynomial basis C5/2 at current scale
    double QuarkInputd[2][nGegen];
    double GluonInputd[2][nGegen];
    double QuarkChQSMd[nGegen]; // Coefficients of the D-term expansion over Gegenbauer polynomial basis C3/2 at initial scale
    double GluonChQSMd[nGegen]; // Coefficients of the D-term expansion over Gegenbauer polynomial basis C5/2 at initial scale

    ////////////////////////////////////
    //   Beta function coefficients   //
    ////////////////////////////////////

    // 3 active flavours
    NFlavour = 3;
    Beta0[0] = (11. / 3. * m_CA - 4. / 3. * m_TF * NFlavour) / 4.;
    Beta1[0] = (34. / 3. * CA2 - 20. / 3. * m_CA * m_TF * NFlavour
            - 4. * m_CF * m_TF * NFlavour) / 16.;

    // 4 active flavours

    NFlavour = 4;
    Beta0[1] = (11. / 3. * m_CA - 4. / 3. * m_TF * NFlavour) / 4.;
    Beta1[1] = (34. / 3. * CA2 - 20. / 3. * m_CA * m_TF * NFlavour
            - 4. * m_CF * m_TF * NFlavour) / 16.;

    //////////////////////////////////////////////////////
    //   Strong Running coupling at initial scale Mu0   //
    //////////////////////////////////////////////////////
    Lambda = Lambda3;
    t = 2. * log(Mu0 / Lambda);
    lt = log(t);

    AlphaS0 = 1. / (Beta0[0] * t); // - Beta1[ 0 ] * lt / ( ( Beta0[ 0 ] * t ) * ( Beta0[ 0 ] * t ) ) ;
    AlphaS0 *= PI;

    //////////////////////////////////////////////////////////////////////////
    //   Strong Running coupling at charm threshold MuActiveFlavourSwitch   //
    //////////////////////////////////////////////////////////////////////////
    Lambda = Lambda3;
    t = 2. * log(MuSwitch / Lambda);
    lt = log(t);

    AlphaSMuSwitch = 1. / (Beta0[0] * t); // - Beta1[ 0 ] * lt / ( ( Beta0[ 0 ] * t ) * ( Beta0[ 0 ] * t ) ) ;
    AlphaSMuSwitch *= PI;

    ///////////////////////////////////////////////////////
    //   Strong Running coupling at current scale fMuR   //
    ///////////////////////////////////////////////////////
    if (m_MuR2 <= MuSwitch) {
        Lambda = Lambda3;
        t = 2. * log(m_MuR2 / Lambda);
        lt = log(t);
        AlphaS = 1. / (Beta0[0] * t); // - Beta1[ 0 ] * lt / ( ( Beta0[ 0 ] * t ) * ( Beta0[ 0 ] * t ) ) ;
    } else {
        Lambda = Lambda4;
        t = 2. * log(m_MuR2 / Lambda);
        lt = log(t);
        AlphaS = 1. / (Beta0[1] * t); // - Beta1[  1 ] * lt / ( ( Beta0[ 1 ] * t ) * ( Beta0[ 1 ] * t ) ) ;
    }
    AlphaS *= PI;

    /////////////////////////////////////
    //   Gegenbauer polynomials C3/2   //
    /////////////////////////////////////

    // C^(3/2)_1
    QuarkGegenbauer[0] = 3. * z;

    // C^(3/2)_3
    QuarkGegenbauer[1] = 5. / 2. * (-3. * z + 7. * pow(z, 3));

    // C^(3/2)_5
    QuarkGegenbauer[2] = 21. / 8.
            * (5. * z - 30. * pow(z, 3) + 33. * pow(z, 5));

    /////////////////////////////////////
    //   Gegenbauer polynomials C5/2   //
    /////////////////////////////////////

    // C^(5/2)_0
    GluonGegenbauer[0] = 1.;

    // C^(5/2)_2
    GluonGegenbauer[1] = 5. / 2. * (-1. + 7. * pow(z, 2));

    // C^(5/2)_4
    GluonGegenbauer[2] = 35. / 8. * (1. - 18. * pow(z, 2) + 33. * pow(z, 4));

    //////////////////////////////
    //   Anomalous dimensions   //
    //////////////////////////////
    for (unsigned int iGegen = 0; iGegen < nGegen; iGegen++) {
        // 3 active flavours
        TabGammaQQ[0][iGegen] = GammaQQ(3, 2 * iGegen + 1);
        TabGammaGQ[0][iGegen] = GammaGQ(3, 2 * iGegen + 1);
        TabGammaQG[0][iGegen] = GammaQG(3, 2 * iGegen + 1);
        TabGammaGG[0][iGegen] = GammaGG(3, 2 * iGegen + 1);

        // 4 active flavours
        TabGammaQQ[1][iGegen] = GammaQQ(4, 2 * iGegen + 1);
        TabGammaGQ[1][iGegen] = GammaGQ(4, 2 * iGegen + 1);
        TabGammaQG[1][iGegen] = GammaQG(4, 2 * iGegen + 1);
        TabGammaGG[1][iGegen] = GammaGG(4, 2 * iGegen + 1);

        for (unsigned int i = 0; i < 2; i++) {
            TabGammaPrimeQG[i][iGegen] = (2. * iGegen + 1.) / 3.
                    * TabGammaQG[i][iGegen];
            TabGammaPrimeGQ[i][iGegen] = 3. / (2. * iGegen + 1.)
                    * TabGammaGQ[i][iGegen];

            CapitalGammap[i][iGegen] = (TabGammaQQ[i][iGegen]
                    - TabGammaGG[i][iGegen])
                    * (TabGammaQQ[i][iGegen] - TabGammaGG[i][iGegen]);
            CapitalGammap[i][iGegen] += 4. * TabGammaPrimeQG[i][iGegen]
                    * TabGammaPrimeGQ[i][iGegen];
            CapitalGammap[i][iGegen] = sqrt(CapitalGammap[i][iGegen]);
            CapitalGammap[i][iGegen] += TabGammaQQ[i][iGegen]
                    + TabGammaGG[i][iGegen];
            CapitalGammap[i][iGegen] /= 2.;

            CapitalGammam[i][iGegen] = (TabGammaQQ[i][iGegen]
                    - TabGammaGG[i][iGegen])
                    * (TabGammaQQ[i][iGegen] - TabGammaGG[i][iGegen]);
            CapitalGammam[i][iGegen] += 4. * TabGammaPrimeQG[i][iGegen]
                    * TabGammaPrimeGQ[i][iGegen];
            CapitalGammam[i][iGegen] = -sqrt(CapitalGammam[i][iGegen]);
            CapitalGammam[i][iGegen] += TabGammaQQ[i][iGegen]
                    + TabGammaGG[i][iGegen];
            CapitalGammam[i][iGegen] /= 2.;

            gp[i][iGegen] = CapitalGammap[i][iGegen] - TabGammaQQ[i][iGegen];
            gp[i][iGegen] /= TabGammaPrimeQG[i][iGegen];

            gm[i][iGegen] = CapitalGammam[i][iGegen] - TabGammaQQ[i][iGegen];
            gm[i][iGegen] /= TabGammaPrimeQG[i][iGegen];

//          Kp[i][iGegen] = 2. * CapitalGammap[i][iGegen] / Beta1[i];
//          Km[i][iGegen] = 2. * CapitalGammam[i][iGegen] / Beta1[i];
            Kp[i][iGegen] = 2. * CapitalGammap[i][iGegen] / (4. * Beta0[i]);
            Km[i][iGegen] = 2. * CapitalGammam[i][iGegen] / (4. * Beta0[i]);
        }
    }

    //////////////////////////////////////////////////////////////////
    //   Coefficients initialized with Chiral Quark Soliton Model   //
    //////////////////////////////////////////////////////////////////

    QuarkChQSMd[0] = 4.0;
    QuarkChQSMd[1] = 1.2;
    QuarkChQSMd[2] = 0.4;

    GluonChQSMd[0] = 0.;
    GluonChQSMd[1] = 0.;
    GluonChQSMd[2] = 0.;

    for (unsigned int iGegen = 0; iGegen < nGegen; iGegen++) {
        QuarkInputd[0][iGegen] = QuarkChQSMd[iGegen];
        GluonInputd[0][iGegen] = GluonChQSMd[iGegen];
    }

    /////////////////////////////////////
    //   Scale dependent coefficients  //
    /////////////////////////////////////
    for (unsigned int iGegen = 0; iGegen < nGegen; iGegen++) {

        // Find coefficients Ap and Am at input scale Mu0
        Ap[0][iGegen] = (gm[0][iGegen] * QuarkInputd[0][iGegen]
                - GluonInputd[0][iGegen]) / (gm[0][iGegen] - gp[0][iGegen]);
        Am[0][iGegen] = (gp[0][iGegen] * QuarkInputd[0][iGegen]
                - GluonInputd[0][iGegen]) / (gp[0][iGegen] - gm[0][iGegen]);

        // Compute expansion into Gegenbauer polynomials at Mu = MuSwitch
        QuarkInputd[1][iGegen] = Ap[0][iGegen]
                * pow(AlphaSMuSwitch / AlphaS0, Kp[0][iGegen]);
        QuarkInputd[1][iGegen] += Am[0][iGegen]
                * pow(AlphaSMuSwitch / AlphaS0, Km[0][iGegen]);

        GluonInputd[1][iGegen] = gp[0][iGegen] * Ap[0][iGegen]
                * pow(AlphaSMuSwitch / AlphaS0, Kp[0][iGegen]);
        GluonInputd[1][iGegen] += gm[0][iGegen] * Am[0][iGegen]
                * pow(AlphaSMuSwitch / AlphaS0, Km[0][iGegen]);

        // Find coefficients Ap and Am at Mu = MuSwitch
        Ap[1][iGegen] = (gm[1][iGegen] * QuarkInputd[1][iGegen]
                - GluonInputd[1][iGegen]) / (gm[1][iGegen] - gp[1][iGegen]);
        Am[1][iGegen] = (gp[1][iGegen] * QuarkInputd[1][iGegen]
                - GluonInputd[1][iGegen]) / (gp[1][iGegen] - gm[1][iGegen]);

        if (m_MuR2 <= MuSwitch) {
            Quarkd[iGegen] = Ap[0][iGegen]
                    * pow(AlphaS / AlphaS0, Kp[0][iGegen]);
            Quarkd[iGegen] += Am[0][iGegen]
                    * pow(AlphaS / AlphaS0, Km[0][iGegen]);

            Gluond[iGegen] = gp[0][iGegen] * Ap[0][iGegen]
                    * pow(AlphaS / AlphaS0, Kp[0][iGegen]);
            Gluond[iGegen] += gm[0][iGegen] * Am[0][iGegen]
                    * pow(AlphaS / AlphaS0, Km[0][iGegen]);
        } else {
            Quarkd[iGegen] = Ap[1][iGegen]
                    * pow(AlphaS / AlphaSMuSwitch, Kp[1][iGegen]);
            Quarkd[iGegen] += Am[1][iGegen]
                    * pow(AlphaS / AlphaSMuSwitch, Km[1][iGegen]);

            Gluond[iGegen] = gp[1][iGegen] * Ap[1][iGegen]
                    * pow(AlphaS / AlphaSMuSwitch, Kp[1][iGegen]);
            Gluond[iGegen] += gm[1][iGegen] * Am[1][iGegen]
                    * pow(AlphaS / AlphaSMuSwitch, Km[1][iGegen]);
        }

//      cout << "Quarkd[" << iGegen << "] = " << Quarkd[iGegen] << endl ;
//      cout << "Gluond[" << iGegen << "] = " << Gluond[iGegen] << endl ;
    }

    ////////////////
    //   D-terms  //
    ////////////////
    m_QuarkDTerm = 0.;
    m_GluonDTerm = 0.;
    if (fabs(z) < 1.) {
        for (unsigned int iGegen = 0; iGegen < nGegen; iGegen++) {
            m_QuarkDTerm += Quarkd[iGegen] * QuarkGegenbauer[iGegen];
            m_GluonDTerm += Gluond[iGegen] * GluonGegenbauer[iGegen];
        }

        // Normalize with appropriate weight
        m_QuarkDTerm *= -(1. - z * z);
        m_GluonDTerm *= -3. / 2. * (1. - z * z) * (1. - z * z);

        // Add t-dependence
        m_QuarkDTerm *= m_FD / 3.;
        m_GluonDTerm *= m_FD * m_xi;
    }
}

void MPSSW13WithDTermModel::ComputeFormFactors() {
    double Gep, Gen, Gmp, Gmn; // Sachs form factors
    double F1p, F1n, F2p, F2n; // Pauli-Dirac form factors

    const double Mup = 2.79278; // Proton magnetic moment (Nucl. Phys. B32 (1971) 221)
    const double Mun = -1.91315; // Neutron magnetic moment (Nucl. Phys. B32 (1971) 221)
    const double MassScaleProton2 = 0.71; // Mass scale for t-dep of Gep (in GeV^2)
    const double MV2 = 0.84 * 0.84;
    double tau = m_t / (4. * PROTON_MASS * PROTON_MASS);

    // Sachs form factors
    Gep = 1. / ((1. - m_t / MV2) * (1. - m_t / MV2));
    Gmp = Mup * Gep;

    Gmn = Mun * Gep;
    Gen = -tau / (1 - 5.6 * tau) * fabs(Gmn);

    // Pauli-Dirac from factors
    F1p = (Gep - Gmp * tau) / (1. - tau);
    F2p = (Gmp - Gep) / (1. - tau);

    F1n = (Gen - Gmn * tau) / (1. - tau);
    F2n = (Gmn - Gen) / (1. - tau);

    // Quark contributions to Pauli-Dirac fom factors
    m_F1u = 2. * F1p + F1n;
    m_F1d = F1p + 2. * F1n;

    m_FD = 1. / ((1. - m_t / MV2) * (1. - m_t / MV2));
}

double MPSSW13WithDTermModel::Profile(double N, double beta, double alpha) {
    double profile;
    double ProfileShape = N;
    double TwiceProfileShapePlus1 = 2. * ProfileShape + 1;

//    double alphaBeta = fabs(alpha) + fabs(beta);
//    if (alphaBeta > 1.) {
//        error(__func__,
//                ElemUtils::Formatter()
//                        << "MPSSW13WithDTermModel : Parameters of profile function should be in rhombus | alpha | + | beta | <= 1."
//                        << '\n' << "Here alpha = " << alpha << " beta = "
//                        << beta << " | alpha | + | beta | = " << alphaBeta
//                        << '\n');
//    }

    profile = pow((1. - fabs(beta)) * (1. - fabs(beta)) - alpha * alpha,
            ProfileShape);
    profile /= pow(1. - fabs(beta), TwiceProfileShapePlus1);
    profile *= tgamma(TwiceProfileShapePlus1 + 1.);
    profile /= (pow(2., TwiceProfileShapePlus1) * tgamma(ProfileShape + 1.)
            * tgamma(ProfileShape + 1.));

    return profile;
}

void MPSSW13WithDTermModel::throwBetaException(const std::string &funcName,
        double betaValue) {
    throw ElemUtils::CustomException(getClassName(), funcName,
            ElemUtils::Formatter()
                    << "Longitudinal momentum fraction should be in ] 0., +1. ]"
                    << '\n' << "Here beta = " << betaValue << '\n');
}

double MPSSW13WithDTermModel::IntegralHuVal(double x, std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetuVal();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    if (beta > 0) {
        pdf = m_Forward->getPartonContent().getUpv() / absbeta;
    } else {
        pdf = 0.;
    }
    Integral = pdf * Profile(m_ProfileShapeVal, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1u / 2.;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralHuValMx(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetuVal();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    if (beta > 0) {
        pdf = m_Forward->getPartonContent().getUpv() / absbeta;
    } else {
        pdf = 0.;
    }
    Integral = pdf * Profile(m_ProfileShapeVal, beta, (m_Mx - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1u / 2.;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHuSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetuSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getUsea() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1u / 2.;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHuSeaMx(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetuSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getUsea() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x + beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1u / 2.;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall1HuSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetuSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getUsea() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1u / 2.;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall2HuSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetuSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getUsea() / absbeta;

    /*    m_Forward->update(absbeta, sqrt(m_MuF2));
     if (beta > 0) {
     pdf = m_Forward->getPartonContent().getUpv() / absbeta;
     } else {

     }*/ //  Possible source of the error, J.Wagner
    Integral = Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral += -Profile(m_ProfileShapeSea, beta, (m_x + beta) / m_xi);
    Integral *= pdf;
    Integral /= m_xi;
    Integral *= m_F1u / 2.;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralHdVal(double x, std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdVal();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    if (beta > 0) {
        pdf = m_Forward->getPartonContent().getDnv() / absbeta;
    } else {
        pdf = 0.;
    }
    Integral = pdf * Profile(m_ProfileShapeVal, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1d;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralHdValMx(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdVal();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    if (beta > 0) {
        pdf = m_Forward->getPartonContent().getDnv() / absbeta;
    } else {
        pdf = 0.;
    }
    Integral = pdf * Profile(m_ProfileShapeVal, beta, (m_Mx - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1d;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHdSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getDsea() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1d;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHdSeaMx(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getDsea() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x + beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1d;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall1HdSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getDsea() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_F1d;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall2HdSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getDsea() / absbeta;
    Integral = Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral += -Profile(m_ProfileShapeSea, beta, (m_x + beta) / m_xi);
    Integral *= pdf;
    Integral /= m_xi;
    Integral *= m_F1d;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHsSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getSbar() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHsSeaMx(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->GetdSea();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getSbar() / absbeta;
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x + beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall1HsSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->Gets();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    if (beta > 0) {
        pdf = m_Forward->getPartonContent().getSbar() / absbeta;
    } else {
        pdf = -m_Forward->getPartonContent().getSbar() / absbeta;
    }
    Integral = pdf * Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall2HsSea(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->Gets();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    if (beta > 0) {
        pdf = m_Forward->getPartonContent().getSbar() / absbeta;
    } else {
        pdf = -m_Forward->getPartonContent().getSbar() / absbeta;
    }
    Integral = Profile(m_ProfileShapeSea, beta, (m_x - beta) / m_xi);
    Integral += -Profile(m_ProfileShapeSea, beta, (m_x + beta) / m_xi);
    Integral *= pdf;
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHg(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->Getg();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getGlu() / absbeta;
    Integral = pdf * beta
            * Profile(m_ProfileShapeGlue, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxLargeHgMx(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->Getg();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getGlu() / absbeta;
    Integral = pdf * beta
            * Profile(m_ProfileShapeGlue, beta, (m_x + beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall1Hg(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->Getg();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getGlu() / absbeta;
    Integral = pdf * beta
            * Profile(m_ProfileShapeGlue, beta, (m_x - beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::IntegralxSmall2Hg(double x,
        std::vector<double> Par) {
    double Integral;
    double pdf, beta, absbeta;

    beta = x;
    absbeta = fabs(beta);

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, x);
    }

//  m_Forward->Setx( beta );
//  pdf = m_Forward->Getg();
    m_Forward->update(absbeta, sqrt(m_MuF2));
    pdf = m_Forward->getPartonContent().getGlu() / absbeta;
    Integral = pdf * beta
            * Profile(m_ProfileShapeSea, beta, (m_x + beta) / m_xi);
    Integral /= m_xi;
    Integral *= m_FD;

    return Integral;
}

double MPSSW13WithDTermModel::GammaQQ(const unsigned int nflavour,
        const unsigned int n) {
    double Gamma;
    double HarmonicSum = 0.;

    for (unsigned int k = 2; k <= n + 1; k++) {
        HarmonicSum += 1. / k;
    }

    Gamma = 0.5 - 1. / ((n + 1.) * (n + 2.)) + 2. * HarmonicSum;
    Gamma *= m_CF;

    return Gamma;
}

double MPSSW13WithDTermModel::GammaQG(const unsigned int nflavour,
        const unsigned int n) {
    double Gamma;

    Gamma = (n * n + 3. * n + 4.) / (n * (n + 1.) * (n + 2.));
    Gamma *= -1. * nflavour * m_TF;

    return Gamma;
}

double MPSSW13WithDTermModel::GammaGQ(const unsigned int nflavour,
        const unsigned int n) {
    double Gamma;

    Gamma = (n * n + 3. * n + 4.) / ((n + 1.) * (n + 2.) * (n + 3.));
    Gamma *= -2. * m_CF;

    return Gamma;
}

double MPSSW13WithDTermModel::GammaGG(const unsigned int nflavour,
        const unsigned int n) {
    double Gamma;

    double HarmonicSum = 0.;
    for (unsigned int k = 2; k <= n + 1; k++) {
        HarmonicSum += 1. / k;
    }

    Gamma = 1. / 6. - 2. / (n * (n + 1.)) - 2. / ((n + 2.) * (n + 3.))
            + 2. * HarmonicSum;
    Gamma *= m_CA;
    Gamma += 2. / 3. * nflavour * m_TF;

    return Gamma;
}

//TODO meme initialisation que GK11 ?
void MPSSW13WithDTermModel::initModule() {
    GPDModule::initModule();

    debug(__func__, ElemUtils::Formatter() << "fMuF2 = " << m_MuF2);
}

//TODO implement
void MPSSW13WithDTermModel::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();
}

double MPSSW13WithDTermModel::getCA() const {
    return m_CA;
}

double MPSSW13WithDTermModel::getCF() const {
    return m_CF;
}

double MPSSW13WithDTermModel::getF1d() const {
    return m_F1d;
}

double MPSSW13WithDTermModel::getF1u() const {
    return m_F1u;
}

double MPSSW13WithDTermModel::getFD() const {
    return m_FD;
}

double MPSSW13WithDTermModel::getGluonDTerm() const {
    return m_GluonDTerm;
}

void MPSSW13WithDTermModel::setGluonDTerm(double gluonDTerm) {
    m_GluonDTerm = gluonDTerm;
}

double MPSSW13WithDTermModel::getMx() const {
    return m_Mx;
}

unsigned int MPSSW13WithDTermModel::getNbOfColor() const {
    return m_NbOfColor;
}

unsigned int MPSSW13WithDTermModel::getNbOfQuarkFlavor() const {
    return m_NbOfQuarkFlavor;
}

double MPSSW13WithDTermModel::getProfileShapeGlue() const {
    return m_ProfileShapeGlue;
}

double MPSSW13WithDTermModel::getProfileShapeSea() const {
    return m_ProfileShapeSea;
}

double MPSSW13WithDTermModel::getProfileShapeVal() const {
    return m_ProfileShapeVal;
}

double MPSSW13WithDTermModel::getQuarkDTerm() const {
    return m_QuarkDTerm;
}

double MPSSW13WithDTermModel::getTF() const {
    return m_TF;
}

PartonDistribution MPSSW13WithDTermModel::computeH() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Eps = 1.e-9;
    m_Mx = -m_x;

    // Integration limits and methods
    double Beta1 = (m_x - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2 = (m_x + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta1Mx = (m_Mx - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2Mx = (m_Mx + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    // Scales
//  m_Forward->SetFactorizationScale( m_MuF2 );
//  m_Forward->SetRenormalizationScale( m_MuR2 );

    // Form factors
    ComputeFormFactors();

    //compute Dterms
    double quarkDTerm, quarkDTermMx, gluonDTerm;

    ComputeDTerms(m_x);
    quarkDTerm = m_QuarkDTerm;
    gluonDTerm = m_GluonDTerm;

    ComputeDTerms(m_Mx);
    quarkDTermMx = m_QuarkDTerm;

    //////////////////////////////////////////////////////
    //   u and d quarks, valence part evaluated at fx   //
    //////////////////////////////////////////////////////
    double HuVal = 0.;
    double HdVal = 0.;

    std::vector<double> emptyParameters;

    if (m_x >= m_xi) {
        // Integration, u quark
        HuVal = integrate(m_pIntegralHuVal, Beta1, Beta2, emptyParameters);

        // Integration, d quark
        HdVal = integrate(m_pIntegralHdVal, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        // Integration, u quark
        HuVal = integrate(m_pIntegralHuVal, Eps, Beta2, emptyParameters);

        // Integration, d quark
        HdVal = integrate(m_pIntegralHdVal, Eps, Beta2, emptyParameters);
    }

    HuVal += quarkDTerm;
    HdVal += quarkDTerm;

    ///////////////////////////////////////////////////////////////////////
    //   u and d quarks, valence part evaluated at fMx (instead of fx)   //
    ///////////////////////////////////////////////////////////////////////

    double HuValMx = 0.;
    double HdValMx = 0.;

    if (m_Mx >= m_xi) {
        // Integration, u quark
        HuValMx = integrate(m_pIntegralHuValMx, Beta1Mx, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        HdValMx = integrate(m_pIntegralHdValMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    if (fabs(m_Mx) < m_xi) {
        // Integration, u quark
        HuValMx = integrate(m_pIntegralHuValMx, Eps, Beta2Mx, emptyParameters);

        // Integration, d quark
        HdValMx = integrate(m_pIntegralHdValMx, Eps, Beta2Mx, emptyParameters);
    }

    HuValMx += quarkDTermMx;
    HdValMx += quarkDTermMx;

    //////////////////////////////////
    //   u and d quarks, sea part   //
    //////////////////////////////////

    double HuSea = 0.;
    double HdSea = 0.;

    if (m_x >= m_xi) {
        // Integration, u quark
        HuSea = integrate(m_pIntegralxLargeHuSea, Beta1, Beta2,
                emptyParameters);

        // Integration, d quark
        HdSea = integrate(m_pIntegralxLargeHdSea, Beta1, Beta2,
                emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        // Integration, u quark
        HuSea = integrate(m_pIntegralxSmall1HuSea, Beta2Mx, Beta2,
                emptyParameters);

        HuSea += integrate(m_pIntegralxSmall2HuSea, Eps, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        HdSea = integrate(m_pIntegralxSmall1HdSea, Beta2Mx, Beta2,
                emptyParameters);

        HdSea += integrate(m_pIntegralxSmall2HdSea, Eps, Beta2Mx,
                emptyParameters);
    }

    if (m_x <= -m_xi) {
        // Integration, u quark
        HuSea = -integrate(m_pIntegralxLargeHuSeaMx, Beta1Mx, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        HdSea = -integrate(m_pIntegralxLargeHdSeaMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    HuSea += quarkDTerm;
    HdSea += quarkDTerm;

    quarkDistribution_u.setQuarkDistribution(HuVal + HuSea);
    quarkDistribution_d.setQuarkDistribution(HdVal + HdSea);

    //////////////////////////////
    //   s quarks, "sea part"   //
    //////////////////////////////
    double Hs = 0.;

    if (m_x >= m_xi) {
        Hs = integrate(m_pIntegralxLargeHsSea, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        Hs = integrate(m_pIntegralxSmall1HsSea, Beta2Mx, Beta2,
                emptyParameters);

        Hs += integrate(m_pIntegralxSmall2HsSea, Eps, Beta2Mx, emptyParameters);
    }

    if (m_x <= -m_xi) {
        Hs = -integrate(m_pIntegralxLargeHsSeaMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    Hs += quarkDTerm;

    quarkDistribution_s.setQuarkDistribution(Hs);

    ////////////////
    //   Gluons   //
    ////////////////
    double Hg = 0.;

    if (m_x >= m_xi) {
        // Integration
        Hg = integrate(m_pIntegralxLargeHg, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        // Integration
        Hg = integrate(m_pIntegralxSmall1Hg, 0., Beta2, emptyParameters);

        Hg += integrate(m_pIntegralxSmall2Hg, 0., Beta2Mx, emptyParameters);
    }

    if (m_x <= -m_xi) {
        // Integration
        Hg = integrate(m_pIntegralxLargeHgMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    Hg += gluonDTerm;

    GluonDistribution gluonDistribution(Hg);

    //////////////////////////////////////////
    //   H, C-odd and C-even combinations   //
    //////////////////////////////////////////
    // u quark
    quarkDistribution_u.setQuarkDistributionPlus(HuVal - HuValMx + 2 * HuSea);
    quarkDistribution_u.setQuarkDistributionMinus(HuVal + HuValMx);

    // d quark
    quarkDistribution_d.setQuarkDistributionPlus(HdVal - HdValMx + 2 * HdSea);
    quarkDistribution_d.setQuarkDistributionMinus(HdVal + HdValMx);

    // s quark
    quarkDistribution_s.setQuarkDistributionPlus(2 * Hs);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    //TODO no singlet ?

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}
