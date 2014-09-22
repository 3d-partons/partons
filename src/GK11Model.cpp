#include "GK11Model.h"

#include <cln/float.h>
#include <cln/float_class.h>
#include <cln/floatformat.h>
#include <cln/real.h>
#include <cmath>

#include "BaseModuleFactory.h"
#include "models/FundamentalPhysicalConstants.h"
#include "models/GPDOutputData.h"
#include "models/GPDQuarkFlavorData.h"
#include "models/GPDResultData.h"
#include "models/QuarkFlavor.h"
#include "utils/logger/LoggerManager.h"
#include "utils/stringUtils/Formatter.h"

// Initialise GK11GPDModule::ID with a unique name.
const std::string GK11Model::moduleID = "GK11Model";

// Define a useless static boolean variable to enable registerModule() to be executed before the main() function.
// Because global variables have program scope, and are initialised before main() is called.
// !!! CARE !!! variable name must be unique (ex : isGK11ModelRegistered).
static bool isGK11ModelRegistered =
        BaseModuleFactory::getInstance()->registerModule(new GK11Model());

GK11Model::GK11Model() :
        GPDModule(GK11Model::moduleID) {
    init();
}

void GK11Model::init() {
    fL = 0.;
    fQ0 = 4.;
    Huval1tab = std::vector<double>(3, 0.);
    Hdval1tab = std::vector<double>(3, 0.);
    Huval1mtab = std::vector<double>(3, 0.);
    Hdval1mtab = std::vector<double>(3, 0.);
    Hs1tab = std::vector<double>(4, 0.);
    Hi1tab = std::vector<double>(4, 0.);
    Euval1tab = std::vector<double>(2, 0.);
    Edval1tab = std::vector<double>(8, 0.);
    Euval1mtab = std::vector<double>(2, 0.);
    Edval1mtab = std::vector<double>(8, 0.);
    Es1tab = std::vector<double>(3, 0.);
    Ei1tab = std::vector<double>(2, 0.);
    Htuval1tab = std::vector<double>(3, 0.);
    Htdval1tab = std::vector<double>(3, 0.);
    Htuval1mtab = std::vector<double>(3, 0.);
    Htdval1mtab = std::vector<double>(3, 0.);
    Hti1tab = std::vector<double>(3, 0.);
    Etuval1tab = std::vector<double>(4, 0.);
    Etdval1tab = std::vector<double>(4, 0.);
    Etuval1mtab = std::vector<double>(4, 0.);
    Etdval1mtab = std::vector<double>(4, 0.);
}

GK11Model::~GK11Model() {
}

void GK11Model::updateVariables(const double &_MuF) {

    fMuF2 = _MuF * _MuF;
    fL = log(fMuF2 / fQ0); // Logarithmic dependence on the scale

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "fMuF2 = " << fMuF2 << " fL = " << fL);

    //if (m_pLoggerManager->isDebug(getClassName(), __func__)) {
    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "fMuF2 = " << fMuF2 << " fL = " << fL);
    //    }
}

GPDOutputData* GK11Model::compute(const double &_x, const double &_xi,
        const double &_t, const double &_MuF, const double &_MuR,
        GPDComputeType::Type _gpdComputeType) {

    pGPDOutputData = new GPDOutputData();

    // And after, update GK11 variables before computing
    updateVariables(_MuF);

    switch (_gpdComputeType) {
    case GPDComputeType::H: {
        computeH(_x, _xi, _t);
        break;
    }
    case GPDComputeType::Ht: {
        computeHt(_x, _xi, _t);
        break;
    }
    case GPDComputeType::E: {
        computeE(_x, _xi, _t);
        break;
    }
    case GPDComputeType::Et: {
        computeEt(_x, _xi, _t);
        break;
    }
    default: {
        computeH(_x, _xi, _t);
        computeHt(_x, _xi, _t);
        computeE(_x, _xi, _t);
        computeEt(_x, _xi, _t);
    }
    }

    return pGPDOutputData;
}

void GK11Model::computeH(const double &_x, const double &_xi,
        const double &_t) {
    GPDComputeType* pGPDComputeType = new GPDComputeType(GPDComputeType::H);

    GPDResultData* pGPD_H = new GPDResultData(pGPDComputeType);

    GPDQuarkFlavorData* pGPDQuarkFlavorData_u = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::UP));
    GPDQuarkFlavorData* pGPDQuarkFlavorData_d = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::DOWN));
    GPDQuarkFlavorData* pGPDQuarkFlavorData_s = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::STRANGE));

// Scales

    calculateHCoefs(_x, _xi, _t); // Calculate the K's and the coefficients

// Gluons

    c1 = 2.23 + 0.362 * fL; // See table 1 p. 12
    c2 = 5.43 - 7.0 * fL; // See table 1 p. 12
    c3 = -34.0 + 22.5 * fL; // See table 1 p. 12
    c4 = 40.6 - 21.6 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + fMuF2)); // See eq. (39) p. 14

    pGPD_H->setGluon(
            exp(b0 * _t)
                    * (c1 * Hi1tab.at(0) + c2 * Hi1tab.at(1) + c3 * Hi1tab.at(2)
                            + c4 * Hi1tab.at(3))); // See eq. (27)

    // s quark

    c1 = 0.123 + 0.0003 * fL; // See table 1 p. 12
    c2 = -0.327 - 0.004 * fL; // See table 1 p. 12
    c3 = 0.692 - 0.068 * fL; // See table 1 p. 12
    c4 = -0.486 + 0.038 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + fMuF2)); // See eq. (39) p. 14

    pGPDQuarkFlavorData_s->setHq(
            exp(b0 * _t)
                    * (c1 * Hs1tab.at(0) + c2 * Hs1tab.at(1) + c3 * Hs1tab.at(2)
                            + c4 * Hs1tab.at(3))); // See eq. (27)

// u quark, valence part

    c1 = 1.52 + 0.248 * fL; // See eq. (27)
    c2 = 2.88 - 0.94 * fL; // See eq. (27)
    c3 = -0.095 * fL; // See eq. (27)

    b0 = 0; // See p. 15

    pGPDQuarkFlavorData_u->setValence(
            exp(b0 * _t)
                    * (c1 * Huval1tab.at(0) + c2 * Huval1tab.at(1)
                            + c3 * Huval1tab.at(2))); // See eq. (27)

    fHuValMx = exp(b0 * _t)
            * (c1 * Huval1mtab.at(0) + c2 * Huval1mtab.at(1)
                    + c3 * Huval1mtab.at(2));  // See eq. (27);

// d quark, valence part

    c1 = 0.76 + 0.248 * fL; // See table 1 p. 12
    c2 = 3.11 - 1.36 * fL; // See table 1 p. 12
    c3 = -3.99 + 1.15 * fL; // See table 1 p. 12

    b0 = 0; // See p. 15

    pGPDQuarkFlavorData_d->setValence(
            exp(b0 * _t)
                    * (c1 * Hdval1tab.at(0) + c2 * Hdval1tab.at(1)
                            + c3 * Hdval1tab.at(2))); // See eq. (27)
    fHdValMx = exp(b0 * _t)
            * (c1 * Hdval1mtab.at(0) + c2 * Hdval1mtab.at(1)
                    + c3 * Hdval1mtab.at(2)); // See eq. (27)

// u and d quarks, sea part

    kappa_s = 1. + 0.68 / (1. + 0.52 * log(fMuF2 / fQ0)); // See eq. (36)

    pGPDQuarkFlavorData_u->setSea(kappa_s * pGPDQuarkFlavorData_s->getHq()); // See eq. (35)
    pGPDQuarkFlavorData_d->setSea(pGPDQuarkFlavorData_u->getSea());

    // u and d quarks, valence + sea parts

    if (_x > 0.) {

        pGPDQuarkFlavorData_u->setHq(
                pGPDQuarkFlavorData_u->getValence()
                        + pGPDQuarkFlavorData_u->getSea());
        pGPDQuarkFlavorData_d->setHq(
                pGPDQuarkFlavorData_d->getValence()
                        + pGPDQuarkFlavorData_d->getSea());

    } else {

        pGPDQuarkFlavorData_u->setHq(-pGPDQuarkFlavorData_u->getSea());
        pGPDQuarkFlavorData_d->setHq(-pGPDQuarkFlavorData_d->getSea());

    }

// H, charge singlet

//    pGPD_H->setSquareChargeAveraged(
//            U2_ELEC_CHARGE
//                    * (pGPDQuarkFlavorData_u->getSea()
//                            + pGPDQuarkFlavorData_u->getValence())
//                    + D2_ELEC_CHARGE
//                            * (pGPDQuarkFlavorData_d->getSea()
//                                    + pGPDQuarkFlavorData_d->getValence())
//                    + S2_ELEC_CHARGE * pGPDQuarkFlavorData_s->getHq());

    pGPDQuarkFlavorData_u->setSinglet(
            pGPDQuarkFlavorData_u->getValence() - fHuValMx
                    + 2. * pGPDQuarkFlavorData_u->getSea());

    pGPDQuarkFlavorData_d->setSinglet(
            pGPDQuarkFlavorData_d->getValence() - fHdValMx
                    + 2. * pGPDQuarkFlavorData_d->getSea());

    pGPDQuarkFlavorData_s->setSinglet(2. * pGPDQuarkFlavorData_s->getHq());

//TODO modifier ce calcul
    pGPD_H->setSinglet(
            U2_ELEC_CHARGE
                    * (2. * pGPDQuarkFlavorData_u->getSea()
                            + pGPDQuarkFlavorData_u->getValence() - fHuValMx)
                    + D2_ELEC_CHARGE
                            * (2. * pGPDQuarkFlavorData_d->getSea()
                                    + pGPDQuarkFlavorData_d->getValence()
                                    - fHdValMx)
                    + S2_ELEC_CHARGE * 2. * pGPDQuarkFlavorData_s->getHq()); // True result !
                            //  fHSinglet = U2_ELEC_CHARGE * ( fHuVal - fHuValMx ) + D2_ELEC_CHARGE * ( fHdVal - fHdValMx ) ; // No sea, compare to MMS

    pGPD_H->addGPDQuarkFlavorData(pGPDQuarkFlavorData_u);
    pGPD_H->addGPDQuarkFlavorData(pGPDQuarkFlavorData_d);
    pGPD_H->addGPDQuarkFlavorData(pGPDQuarkFlavorData_s);

    pGPDOutputData->addGPDResultData(pGPD_H);
}

void GK11Model::computeHt(const double &_x, const double &_xi,
        const double &_t) {

    m_pLoggerManager->info(getClassName(), __func__, "Entered function ...");

    GPDComputeType* pGPDComputeType = new GPDComputeType(GPDComputeType::Ht);

    GPDResultData* pGPD_Ht = new GPDResultData(pGPDComputeType);

    GPDQuarkFlavorData* pGPDQuark_u = new GPDQuarkFlavorData(pGPDComputeType,
            new QuarkFlavor(QuarkFlavor::UP));
    GPDQuarkFlavorData* pGPDQuark_d = new GPDQuarkFlavorData(pGPDComputeType,
            new QuarkFlavor(QuarkFlavor::DOWN));
    GPDQuarkFlavorData* pGPDQuark_s = new GPDQuarkFlavorData(pGPDComputeType,
            new QuarkFlavor(QuarkFlavor::STRANGE));

    double delta, etau, etad, Nu, Nd;

    calculateHtCoefs(_x, _xi, _t); // Calculate the K's and the coefficients

    // Gluons

    c1 = 3.39 - 0.864 * fL;
    c2 = 1.73 + 0.24 * fL - 0.17 * fL * fL;
    c3 = 0.42 - 0.115 * fL - 0.069 * fL * fL;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + fMuF2));

    pGPD_Ht->setGluon(
            exp(b0 * _t)
                    * (c1 * Hti1tab.at(0) + c2 * Hti1tab.at(1)
                            + c3 * Hti1tab.at(2)));

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "c1 = " << c1 << ", c2 = " << c2 << ", c3 = " << c3
                    << ", b0 = " << b0);

    // s quark,  Ht_sea = 0 for GK

    pGPDQuark_s->setHq(0.);

    // u quark, valence part

    c1 = 0.170 + 0.03 * fL; //parameters from GK3 table 4  and note 11 december 2008
    c2 = 1.340 - 0.02 * fL;
    c3 = 0.120 - 0.40 * fL;

    etau = 0.926;
    delta = 0.48;
    b0 = 0.;

    Nu = tgamma(1. - delta) * tgamma(4.) / tgamma(5. - delta)
            * (c1 + c2 * (1. - delta) / (5. - delta)
                    + c3 * (2. - delta) * (1. - delta) / (6 - delta)
                            / (5. - delta));
    //      +  c4 * (3.-delta) * (2.-delta) * (1-delta)/(7.-delta)/(6.-delta)/(5.-delta) ) ;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "c1 = " << c1 << ", c2 = " << c2 << ", c3 = " << c3
                    << ", b0 = " << b0 << ", etau = " << etau << ", delta = "
                    << delta << ", Nu = " << Nu);

    pGPDQuark_u->setValence(
            etau / Nu * exp(b0 * _t)
                    * (c1 * Htuval1tab.at(0) + c2 * Htuval1tab.at(1)
                            + c3 * Htuval1tab.at(2)));
    fHtuValMx = etau / Nu * exp(b0 * _t)
            * (c1 * Htuval1mtab.at(0) + c2 * Htuval1mtab.at(1)
                    + c3 * Htuval1mtab.at(2)); // -pGPDData->getX()

    // d quark, valence part

    c1 = -0.320 - 0.040 * fL;    // parameters from GK3 table 4
    c2 = -1.427 - 0.176 * fL;
    c3 = 0.692 - 0.068 * fL;

    etad = -0.341;

    Nd = tgamma(1. - delta) * tgamma(4.) / tgamma(5. - delta)
            * (c1 + c2 * (1. - delta) / (5. - delta)
                    + c3 * (2. - delta) * (1. - delta) / (6. - delta)
                            / (5. - delta));

    b0 = 0;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "c1 = " << c1 << ", c2 = " << c2 << ", c3 = " << c3
                    << ", b0 = " << b0 << ", etad = " << etad << ", Nd = "
                    << Nd);

    pGPDQuark_d->setValence(
            etad / Nd * exp(b0 * _t)
                    * (c1 * Htdval1tab.at(0) + c2 * Htdval1tab.at(1)
                            + c3 * Htdval1tab.at(2)));
    fHtdValMx = etad / Nd * exp(b0 * _t)
            * (c1 * Htdval1mtab.at(0) + c2 * Htdval1mtab.at(1)
                    + c3 * Htdval1mtab.at(2)); // -pGPDData->getX()

    // u and d quark, sea part

    pGPDQuark_u->setSea(0.);
    pGPDQuark_d->setSea(0.);

    // u and d quarks, valence + sea parts

    if (_x > 0.) {
        pGPDQuark_u->setHq(pGPDQuark_u->getValence() + pGPDQuark_u->getSea());
        pGPDQuark_d->setHq(pGPDQuark_d->getValence() + pGPDQuark_d->getSea());
    } else {
        pGPDQuark_u->setHq(-pGPDQuark_u->getSea());
        pGPDQuark_d->setHq(-pGPDQuark_d->getSea());
    }

    // Ht, charge singlet

    pGPDQuark_u->setSinglet(pGPDQuark_u->getValence() + fHtuValMx);
    pGPDQuark_d->setSinglet(pGPDQuark_d->getValence() + fHtdValMx);
    pGPDQuark_s->setSinglet(0.);

    //TODO a remplacer par 1 / ( 2 NFlavor ) ( HtuSinglet + HtdSinglet + HtsSinglet )
    pGPD_Ht->setSinglet(
            U2_ELEC_CHARGE * (pGPDQuark_u->getSinglet())
                    + D2_ELEC_CHARGE * (pGPDQuark_d->getSinglet()));

    pGPD_Ht->addGPDQuarkFlavorData(pGPDQuark_u);
    pGPD_Ht->addGPDQuarkFlavorData(pGPDQuark_d);
    pGPD_Ht->addGPDQuarkFlavorData(pGPDQuark_s);

    pGPDOutputData->addGPDResultData(pGPD_Ht);
}

void GK11Model::computeE(const double &_x, const double &_xi,
        const double &_t) {
    GPDComputeType* pGPDComputeType = new GPDComputeType(GPDComputeType::E);

    GPDResultData* pGPD_E = new GPDResultData(pGPDComputeType);

    GPDQuarkFlavorData* pGPDQuarkFlavorData_u = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::UP));
    GPDQuarkFlavorData* pGPDQuarkFlavorData_d = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::DOWN));
    GPDQuarkFlavorData* pGPDQuarkFlavorData_s = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::STRANGE));

    calculateECoefs(_x, _xi, _t); // Calculate the K's and the coefficients

    // Gluons

    c1 = 0.779;
    c2 = -c1;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + fMuF2));

    pGPD_E->setGluon(exp(b0 * _t) * (c1 * Ei1tab.at(0) + c2 * Ei1tab.at(1)));

    // s quark (sea)

    c1 = -0.155;
    c2 = -2 * c1;
    c3 = c1;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + fMuF2));

    pGPDQuarkFlavorData_s->setHq(
            exp(b0 * _t)
                    * (c1 * Es1tab.at(0) + c2 * Es1tab.at(1) + c3 * Es1tab.at(2)));

    // u quark, valence part

    c1 = 2.2053;
    c2 = -c1;

    b0 = 0;

    pGPDQuarkFlavorData_u->setValence(
            exp(b0 * _t) * (c1 * Euval1tab.at(0) + c2 * Euval1tab.at(1)));
    fEuValMx = exp(b0 * _t) * (c1 * Euval1mtab.at(0) + c2 * Euval1mtab.at(1)); // -pGPDData->getX()

    // d quark, valence part

    c1 = -3.114; // DFJK4 version betad = 5.6
    c2 = 8.096;
    c3 = -6.477;
    c4 = 1.295;
    c5 = 0.1296;
    c6 = 0.0362;
    c7 = 0.014516;
    c8 = 0.0070504;

    b0 = 0;

    pGPDQuarkFlavorData_d->setValence(
            exp(b0 * _t)
                    * (c1 * Edval1tab.at(0) + c2 * Edval1tab.at(1)
                            + c3 * Edval1tab.at(2) + c4 * Edval1tab.at(3)
                            + c5 * Edval1tab.at(4) + c6 * Edval1tab.at(5)
                            + c7 * Edval1tab.at(6) + c8 * Edval1tab.at(7)));
    fEdValMx = exp(b0 * _t)
            * (c1 * Edval1mtab.at(0) + c2 * Edval1mtab.at(1)
                    + c3 * Edval1mtab.at(2) + c4 * Edval1mtab.at(3)
                    + c5 * Edval1mtab.at(4) + c6 * Edval1mtab.at(5)
                    + c7 * Edval1mtab.at(6) + c8 * Edval1mtab.at(7)); // -pGPDData->getX()

    // u and d quarks, sea part

    //EuSea = Es
    pGPDQuarkFlavorData_u->setSea(pGPDQuarkFlavorData_s->getHq());
    //EdSea = Es
    pGPDQuarkFlavorData_d->setSea(pGPDQuarkFlavorData_s->getHq());

    // u and d quarks, valence + sea parts

    if (_x > 0.) {
        //Eu = EuVal + EuSea
        pGPDQuarkFlavorData_u->setHq(
                pGPDQuarkFlavorData_u->getValence()
                        + pGPDQuarkFlavorData_u->getSea());
        //Ed = EdVal + EdSea
        pGPDQuarkFlavorData_d->setHq(
                pGPDQuarkFlavorData_d->getValence()
                        + pGPDQuarkFlavorData_d->getSea());
    } else {
        //Eu = -EuSea
        pGPDQuarkFlavorData_u->setHq(-pGPDQuarkFlavorData_u->getSea());
        //Ed = -EdSea
        pGPDQuarkFlavorData_d->setHq(-pGPDQuarkFlavorData_d->getSea());
    }

    // E, charge singlet

    // EuSinglet = EuVal - EuValMx + 2. * EuSea
    pGPDQuarkFlavorData_u->setSinglet(
            pGPDQuarkFlavorData_u->getValence() - fEuValMx
                    + 2. * pGPDQuarkFlavorData_u->getSea());
    // EdSinglet = EdVal - EdValMx + 2. * EdSea
    pGPDQuarkFlavorData_d->setSinglet(
            pGPDQuarkFlavorData_d->getValence() - fEdValMx
                    + 2. * pGPDQuarkFlavorData_d->getSea());
    // EsSinglet = 2. * Es;
    pGPDQuarkFlavorData_s->setSinglet(2. * pGPDQuarkFlavorData_s->getHq());

    pGPD_E->setSinglet(
            U2_ELEC_CHARGE * pGPDQuarkFlavorData_u->getSinglet()
                    + D2_ELEC_CHARGE * pGPDQuarkFlavorData_d->getSinglet()
                    + S2_ELEC_CHARGE * pGPDQuarkFlavorData_s->getSinglet());

    pGPD_E->addGPDQuarkFlavorData(pGPDQuarkFlavorData_u);
    pGPD_E->addGPDQuarkFlavorData(pGPDQuarkFlavorData_d);
    pGPD_E->addGPDQuarkFlavorData(pGPDQuarkFlavorData_s);

    pGPDOutputData->addGPDResultData(pGPD_E);
}

void GK11Model::computeEt(const double &_x, const double &_xi,
        const double &_t) {
    GPDComputeType* pGPDComputeType = new GPDComputeType(GPDComputeType::Et);

    GPDResultData* pGPD_Et = new GPDResultData(pGPDComputeType);

    GPDQuarkFlavorData* pGPDQuarkFlavorData_u = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::UP));
    GPDQuarkFlavorData* pGPDQuarkFlavorData_d = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::DOWN));
    GPDQuarkFlavorData* pGPDQuarkFlavorData_s = new GPDQuarkFlavorData(
            pGPDComputeType, new QuarkFlavor(QuarkFlavor::STRANGE));

    double Nu, Nd;
    double PionPolePx, PionPoleMx;

    calculateEtCoefs(_x, _xi, _t); // Calculate the K's and the coefficients

    PionPolePx = Et_pole(_xi, _t, _x);
    PionPoleMx = Et_pole(_xi, _t, -_x);

    // Gluons,  Et_gluons = 0 for GK

    pGPD_Et->setGluon(0.);

    // s quark,  Et_sea = 0 for GK

    pGPDQuarkFlavorData_s->setHq(0.);

    // u quark, valence part

    Nu = 14.0;
    c1 = Nu;      // parameters P.Kroll Maple file
    c2 = -2. * Nu;
    c3 = Nu;
    c4 = 0.0;
    b0 = 0.9; // from P.Kroll's email,  14/3/2011

    double fEtuVal = exp(b0 * _t)
            * (c1 * Etuval1tab.at(0) + c2 * Etuval1tab.at(1)
                    + c3 * Etuval1tab.at(2) + c4 * Etuval1tab.at(3));
    fEtuVal += PionPolePx;
    pGPDQuarkFlavorData_u->setValence(fEtuVal);
    fEtuValMx = exp(b0 * _t)
            * (c1 * Etuval1mtab.at(0) + c2 * Etuval1mtab.at(1)
                    + c3 * Etuval1mtab.at(2) + c4 * Etuval1mtab.at(3)); // -pGPDData->getX()
    fEtuValMx += PionPoleMx;

    // d quark, valence part

    Nd = 4.0;
    c1 = Nd;      // parameters P.Kroll Maple file
    c2 = -2. * Nd;
    c3 = Nd;
    c4 = 0.0;
    b0 = 0.9;

    double fEtdVal = exp(b0 * _t)
            * (c1 * Etdval1tab.at(0) + c2 * Etdval1tab.at(1)
                    + c3 * Etdval1tab.at(2) + c4 * Etdval1tab.at(3));
    fEtdVal -= PionPolePx;
    pGPDQuarkFlavorData_d->setValence(fEtdVal);
    fEtdValMx = exp(b0 * _t)
            * (c1 * Etdval1mtab.at(0) + c2 * Etdval1mtab.at(1)
                    + c3 * Etdval1mtab.at(2) + c4 * Etdval1mtab.at(3)); // -pGPDData->getX()
    fEtdValMx -= PionPoleMx;

    // u and d quark, sea part
    pGPDQuarkFlavorData_u->setSea(0.);
    pGPDQuarkFlavorData_d->setSea(0.);

    // u and d quarks, valence + sea parts

    if (_x > 0.) {
        // Etu = EtuVal + EtuSea;
        pGPDQuarkFlavorData_u->setHq(
                pGPDQuarkFlavorData_u->getValence()
                        + pGPDQuarkFlavorData_u->getSea());
        // Etd = EtdVal + EtdSea;
        pGPDQuarkFlavorData_d->setHq(
                pGPDQuarkFlavorData_d->getValence()
                        + pGPDQuarkFlavorData_d->getSea());
    } else {
        // Etu = -EtuSea;
        pGPDQuarkFlavorData_u->setHq(-pGPDQuarkFlavorData_u->getSea());
        // Etd = -EtdSea;
        pGPDQuarkFlavorData_d->setHq(-pGPDQuarkFlavorData_d->getSea());
    }

    // Et, charge singlet part

    // EtuSinglet = EtuVal + EtuValMx;
    pGPDQuarkFlavorData_u->setSinglet(
            pGPDQuarkFlavorData_u->getValence() + fEtuValMx);
    // EtdSinglet = EtdVal + EtdValMx;
    pGPDQuarkFlavorData_d->setSinglet(
            pGPDQuarkFlavorData_d->getValence() + fEtdValMx);
    // EtsSinglet = 0.;
    pGPDQuarkFlavorData_s->setSinglet(0.);

    pGPD_Et->setSinglet(
            U2_ELEC_CHARGE * pGPDQuarkFlavorData_u->getSinglet()
                    + D2_ELEC_CHARGE * pGPDQuarkFlavorData_d->getSinglet());

    pGPD_Et->addGPDQuarkFlavorData(pGPDQuarkFlavorData_u);
    pGPD_Et->addGPDQuarkFlavorData(pGPDQuarkFlavorData_d);
    pGPD_Et->addGPDQuarkFlavorData(pGPDQuarkFlavorData_s);

    pGPDOutputData->addGPDResultData(pGPD_Et);
}

void GK11Model::calculateHCoefs(const double &_x, const double &_xi,
        const double &_t) {
    int slow_sea = 0; // by default,  fast
    int slow_val = 0; // if  =  1 : slow (full calculation with cln )

    if (log10(_xi) < (4. / 2.5) * log10(_x)) {
        slow_sea = 1;
    }

    // For valence

    if ((_xi < 0.01) && (log10(_xi) < ((-4. / log10(0.6)) * log10(_x)))) {
        slow_val = 1;
    }

    calculateHKas(_t); // comes up with kHgluon,  kHsea,  kHuval,  kHdval

    if (!slow_sea) {
        Hs1tab.at(0) = Hs1(_xi, _x, 0., kHsea);
        Hs1tab.at(1) = Hs1(_xi, _x, 0.5, kHsea);
        Hs1tab.at(2) = Hs1(_xi, _x, 1., kHsea);
        Hs1tab.at(3) = Hs1(_xi, _x, 1.5, kHsea);

        Hi1tab.at(0) = Hi1(_xi, _x, 0., kHgluon);
        Hi1tab.at(1) = Hi1(_xi, _x, 0.5, kHgluon);
        Hi1tab.at(2) = Hi1(_xi, _x, 1., kHgluon);
        Hi1tab.at(3) = Hi1(_xi, _x, 1.5, kHgluon);
    } else {
        Hs1tab.at(0) = Hs1_alt(_xi, _x, 0., kHsea);
        Hs1tab.at(1) = Hs1_alt(_xi, _x, 0.5, kHsea);
        Hs1tab.at(2) = Hs1_alt(_xi, _x, 1., kHsea);
        Hs1tab.at(3) = Hs1_alt(_xi, _x, 1.5, kHsea);

        Hi1tab.at(0) = Hi1_alt(_xi, _x, 0., kHgluon);
        Hi1tab.at(1) = Hi1_alt(_xi, _x, 0.5, kHgluon);
        Hi1tab.at(2) = Hi1_alt(_xi, _x, 1., kHgluon);
        Hi1tab.at(3) = Hi1_alt(_xi, _x, 1.5, kHgluon);
    }

    if (!slow_val) {
        Huval1tab.at(0) = Hval1(_xi, _x, 0., kHuval);
        Huval1tab.at(1) = Hval1(_xi, _x, 0.5, kHuval);
        Huval1tab.at(2) = Hval1(_xi, _x, 1., kHuval);

        Hdval1tab.at(0) = Hval1(_xi, _x, 0., kHdval);
        Hdval1tab.at(1) = Hval1(_xi, _x, 0.5, kHdval);
        Hdval1tab.at(2) = Hval1(_xi, _x, 1., kHdval);

        Huval1mtab.at(0) = Hval1(_xi, -_x, 0., kHuval);
        Huval1mtab.at(1) = Hval1(_xi, -_x, 0.5, kHuval);
        Huval1mtab.at(2) = Hval1(_xi, -_x, 1., kHuval);

        Hdval1mtab.at(0) = Hval1(_xi, -_x, 0., kHdval);
        Hdval1mtab.at(1) = Hval1(_xi, -_x, 0.5, kHdval);
        Hdval1mtab.at(2) = Hval1(_xi, -_x, 1., kHdval);

    } else {
        Huval1tab.at(0) = Hval1_alt(_xi, _x, 0., kHuval);
        Huval1tab.at(1) = Hval1_alt(_xi, _x, 0.5, kHuval);
        Huval1tab.at(2) = Hval1_alt(_xi, _x, 1., kHuval);

        Hdval1tab.at(0) = Hval1_alt(_xi, _x, 0., kHdval);
        Hdval1tab.at(1) = Hval1_alt(_xi, _x, 0.5, kHdval);
        Hdval1tab.at(2) = Hval1_alt(_xi, _x, 1., kHdval);

        Huval1mtab.at(0) = Hval1_alt(_xi, -_x, 0., kHuval);
        Huval1mtab.at(1) = Hval1_alt(_xi, -_x, 0.5, kHuval);
        Huval1mtab.at(2) = Hval1_alt(_xi, -_x, 1., kHuval);

        Hdval1mtab.at(0) = Hval1_alt(_xi, -_x, 0., kHdval);
        Hdval1mtab.at(1) = Hval1_alt(_xi, -_x, 0.5, kHdval);
        Hdval1mtab.at(2) = Hval1_alt(_xi, -_x, 1., kHdval);
    }
}

void GK11Model::calculateHKas(const double &_t) {
    double alpha, delta;

    // gluons

    alpha = 0.15;  //slope of gluon trajectory
    delta = 0.10 + 0.06 * fL;
    kHgluon = delta + alpha * _t;

    // sea

    alpha = 0.15;  // same as slope of gluon trajectory
    delta = 1.1 + 0.06 * fL - 0.0027 * fL * fL;
    kHsea = delta + alpha * _t;

    // u valence

    alpha = 0.9;
    delta = 0.48;
    kHuval = delta + alpha * _t;

    // d valence

    alpha = 0.9 - 0.05 * fL;
    delta = 0.48 + 0.006 * fL;
    kHdval = delta + alpha * _t;
}

double GK11Model::Hs1(const double &_xi, double x, double i, double k) {

    double dummy;

    if ((x - _xi) < 0) {
        dummy =
                15. / 4. / pow(_xi, 5.)
                        * pow((x + _xi) / (1. + _xi), 3. + i - k)
                        * ((3. * pow(3. + i - k, 2.)
                                - (1. + i - k) * (5. + i - k))
                                * pow(_xi * _xi - x, 2.)
                                + 6. * (3. + i - k) * _xi * (1. - x)
                                        * (_xi * _xi - x)
                                - 2. * (2. + i - k) * (4. + i - k)
                                        * (1. - _xi * _xi) * (x * x - _xi * _xi))
                        / (1. + i - k) / (2. + i - k) / (3. + i - k)
                        / (4. + i - k) / (5. + i - k)
                        - 15. / 4. / pow(_xi, 5.)
                                * pow((-x + _xi) / (1. + _xi), 3. + i - k)
                                * ((3. * pow(3. + i - k, 2.)
                                        - (1. + i - k) * (5. + i - k))
                                        * pow(_xi * _xi + x, 2.)
                                        + 6. * (3. + i - k) * _xi * (1. + x)
                                                * (_xi * _xi + x)
                                        - 2. * (2. + i - k) * (4. + i - k)
                                                * (1. - _xi * _xi)
                                                * (x * x - _xi * _xi))
                                / (1. + i - k) / (2. + i - k) / (3. + i - k)
                                / (4. + i - k) / (5. + i - k);
    } else {
        dummy = 15. / 4. / pow(_xi, 5.) / (1. + i - k) / (2. + i - k)
                / (3. + i - k) / (4. + i - k) / (5. + i - k)
                * (((3. * pow(3. + i - k, 2.) - (1. + i - k) * (5. + i - k))
                        * pow(_xi * _xi - x, 2.)
                        - 2. * (2. + i - k) * (4. + i - k) * (1. - _xi * _xi)
                                * (x * x - _xi * _xi))
                        * (pow((x + _xi) / (1. + _xi), 3. + i - k)
                                - pow((x - _xi) / (1. - _xi), 3. + i - k))
                        + 6. * (3. + i - k) * _xi * (1. - x) * (_xi * _xi - x)
                                * (pow((x + _xi) / (1. + _xi), 3. + i - k)
                                        + pow((x - _xi) / (1. - _xi),
                                                3. + i - k)));
    }

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "(x=" << x << ", xi=" << _xi << ", i=" << i << ", k="
                    << k << ") dummy = " << dummy);

    return dummy;
}

double GK11Model::Hi1(const double &_xi, double x, double i, double k) {

    double dummy;

    if ((x - _xi) < 0) {
        dummy =
                15. / 4. / pow(_xi, 5.)
                        * pow((x + _xi) / (1. + _xi), 3. + i - k)
                        * ((3. * pow(3. + i - k, 2.)
                                - (1. + i - k) * (5. + i - k))
                                * pow(_xi * _xi - x, 2.)
                                + 6. * (3. + i - k) * _xi * (1. - x)
                                        * (_xi * _xi - x)
                                - 2. * (2. + i - k) * (4. + i - k)
                                        * (1. - _xi * _xi) * (x * x - _xi * _xi))
                        / (1. + i - k) / (2. + i - k) / (3. + i - k)
                        / (4. + i - k) / (5. + i - k)
                        + 15. / 4. / pow(_xi, 5.)
                                * pow((-x + _xi) / (1. + _xi), 3. + i - k)
                                * ((3. * pow(3. + i - k, 2.)
                                        - (1. + i - k) * (5. + i - k))
                                        * pow(_xi * _xi + x, 2.)
                                        + 6. * (3. + i - k) * _xi * (1. + x)
                                                * (_xi * _xi + x)
                                        - 2. * (2. + i - k) * (4. + i - k)
                                                * (1. - _xi * _xi)
                                                * (x * x - _xi * _xi))
                                / (1. + i - k) / (2. + i - k) / (3. + i - k)
                                / (4. + i - k) / (5. + i - k);
    } else {
        dummy = 15. / 4. / pow(_xi, 5.) / (1. + i - k) / (2. + i - k)
                / (3. + i - k) / (4. + i - k) / (5. + i - k)
                * (((3. * pow(3. + i - k, 2.) - (1. + i - k) * (5. + i - k))
                        * pow(_xi * _xi - x, 2.)
                        - 2. * (2. + i - k) * (4. + i - k) * (1. - _xi * _xi)
                                * (x * x - _xi * _xi))
                        * (pow((x + _xi) / (1. + _xi), 3. + i - k)
                                - pow((x - _xi) / (1. - _xi), 3. + i - k))
                        + 6. * (3. + i - k) * _xi * (1. - x) * (_xi * _xi - x)
                                * (pow((x + _xi) / (1. + _xi), 3. + i - k)
                                        + pow((x - _xi) / (1. - _xi),
                                                3. + i - k)));
    }

    return dummy;
}

double GK11Model::Hs1_alt(const double &_xi, double x, double i, double k) {
    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2, dummy3;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi5 = expt(xia, 5);
    dummy2 = exp((3 + ia - ka) * ln((xba + xia) / (1 + xia)));
    if (fabs(x - _xi) > 1e-6) {
        dummy1 = exp((3 + ia - ka) * ln((xba - xia) / (1 - xia)));
        dummy3 = exp((3 + ia - ka) * ln((-xba + xia) / (1 + xia)));
    } else {
        dummy1 = 0;
        dummy3 = 0;
    }

    double dummy;

    if ((x - _xi) < 0) {

        dummya = dummy2
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia - xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 - xba)
                                * (xia * xia - xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya += -dummy3
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia + xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 + xba)
                                * (xia * xia + xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    } else {
        dummya = ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                * expt(xia * xia - xba, 2)
                - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                        * (xba * xba - xia * xia)) * (dummy2 - dummy1)
                + 6 * (3 + ia - ka) * xia * (1 - xba) * (xia * xia - xba)
                        * (dummy2 + dummy1);

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    }
    dummy = double_approx(dummya);

    return dummy;
}

double GK11Model::Hi1_alt(const double &_xi, double x, double i, double k) {
    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2, dummy3;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi5 = expt(xia, 5);

    dummy2 = exp((3 + ia - ka) * ln((xba + xia) / (1 + xia)));

    if (fabs(x - _xi) > 1e-6) {
        dummy1 = exp((3 + ia - ka) * ln((xba - xia) / (1 - xia)));
        dummy3 = exp((3 + ia - ka) * ln((-xba + xia) / (1 + xia)));
    } else {
        dummy1 = 0;
        dummy3 = 0;
    }

    double dummy;

    if ((x - _xi) < 0) {
        dummya = dummy2
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia - xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 - xba)
                                * (xia * xia - xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya += dummy3
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia + xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 + xba)
                                * (xia * xia + xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    } else {
        dummya = ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                * expt(xia * xia - xba, 2)
                - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                        * (xba * xba - xia * xia)) * (dummy2 - dummy1)
                + 6 * (3 + ia - ka) * xia * (1 - xba) * (xia * xia - xba)
                        * (dummy2 + dummy1);

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    }

    dummy = double_approx(dummya);

    return dummy;
}

double GK11Model::Hval1(const double &_xi, double x, double i, double k) {

    double dummy;

    if ((x + _xi) < 0) {
        dummy = 0.; // no anti-quarks in valence !
    } else if ((x - _xi) < 0) {
        dummy = 3. / 2. / pow(_xi, 3.)
                * (pow((x + _xi) / (1. + _xi), (2. + i - k))
                        * (_xi * _xi - x + (2. + i - k) * _xi * (1. - x)))
                / (1. + i - k) / (2. + i - k) / (3. + i - k);
    } else {
        dummy = 3. / 2. / pow(_xi, 3.) / (1. + i - k) / (2. + i - k)
                / (3. + i - k)
                * ((_xi * _xi - x)
                        * (pow((x + _xi) / (1. + _xi), (2. + i - k))
                                - pow((x - _xi) / (1. - _xi), (2. + i - k)))
                        + _xi * (1. - x) * (2. + i - k)
                                * (pow((x + _xi) / (1. + _xi), (2. + i - k))
                                        + pow((x - _xi) / (1. - _xi),
                                                (2. + i - k))));
    }

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "(x=" << x << ", xi=" << _xi << ", i=" << i << ", k="
                    << k << ") dummy = " << dummy);

    return dummy;
}

double GK11Model::Hval1_alt(const double &_xi, double x, double i, double k) {
    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi3 = expt(xia, 3);

    if (fabs(x + _xi) > 1e-6) {
        dummy2 = exp((2 + ia - ka) * ln((xba + xia) / (1 + xia)));
    } else {
        dummy2 = 0;
    }

    if (fabs(x - _xi) > 1e-6) {
        dummy1 = exp((2 + ia - ka) * ln((xba - xia) / (1 - xia)));
    } else {
        dummy1 = 0;
    }

    double dummy;

    if ((x + _xi) < 0) {
        dummy = 0.; // no anti-quarks in valence !
    } else if ((x - _xi) < 0) {
        dummya = dummy2 * (xia * xia - xba + (2 + ia - ka) * xia * (1 - xba));

        dummya *= cl_float(3, prec) / 2 / xi3 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka);

    } else {
        dummya = (xia * xia - xba) * (dummy2 - dummy1);

        dummya += (2 + ia - ka) * xia * (1 - xba) * (dummy2 + dummy1);

        dummya *= cl_float(3, prec) / 2 / xi3 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka);
    }

    dummy = double_approx(dummya);

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "(x=" << x << ", xi=" << _xi << ", i=" << i << ", k="
                    << k << ") dummy = " << dummy);

    return dummy;
}

/** !
 *  \fn void STGK11Model::calculateHtCoefs()
 *
 * Generic function for fast calculation.
 *
 * Calculates :
 *    - Hti1  ( i = 0,  0.5,  1 )
 *    - Htval1( i = 0,  1,  2 )
 *
 * Either fast (cln) or slow, depending on kinematic domain in (x,xi)
 * To be called before the evaluation of GPDs.
 *
 * Kinematic boundaries
 * For the sea and gluons : gluons share the same limits as strange/sea as far as slow/fast goes.
 *    ( gluons only for now,  Htilde_sea = 0 for Kroll )
 *
 */
void GK11Model::calculateHtCoefs(const double &_x, const double &_xi,
        const double &_t) {
    int slow_sea = 0; // by default,  fast
    int slow_val = 0; // if  =  1 : slow ( full calculation with cln )

//TODO difference from source file _xi doit apparement être _eta
    if (log10(_xi) < (4. / 2.5) * log10(_x)) {
        slow_sea = 1;
    }

// For valence

    if ((_xi < 0.01) && (log10(_xi) < ((-4. / log10(0.6)) * log10(_x)))) {
        slow_val = 1;
    }

    calculateHtKas(_t); // comes up with kHtgluon,  kHstea,  kHtuval,  kHtdval

// No sea Ht for GK.

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "slow_sea = " << slow_sea << " slow_val = "
                    << slow_val);

//TODO permuter les conditions

    if (!slow_sea) {
        Hti1tab.at(0) = Hti1(_xi, _x, 0., kHtgluon);
        Hti1tab.at(1) = Hti1(_xi, _x, 0.5, kHtgluon);
        Hti1tab.at(2) = Hti1(_xi, _x, 1.0, kHtgluon);
    } else {
        Hti1tab.at(0) = Hti1_alt(_xi, _x, 0., kHtgluon);
        Hti1tab.at(1) = Hti1_alt(_xi, _x, 0.5, kHtgluon);
        Hti1tab.at(2) = Hti1_alt(_xi, _x, 1.0, kHtgluon);
    }

    if (!slow_val) {
        Htuval1tab.at(0) = Htval1(_xi, _x, 0., kHtuval);
        Htuval1tab.at(1) = Htval1(_xi, _x, 1., kHtuval);
        Htuval1tab.at(2) = Htval1(_xi, _x, 2., kHtuval);

        Htdval1tab.at(0) = Htuval1tab.at(0); // kHtdval & kHtuval are equal
        Htdval1tab.at(1) = Htuval1tab.at(1); // for u and d for Ht
        Htdval1tab.at(2) = Htuval1tab.at(2); // don't need to recalculate

        Htuval1mtab.at(0) = Htval1(_xi, -_x, 0., kHtuval);
        Htuval1mtab.at(1) = Htval1(_xi, -_x, 1., kHtuval);
        Htuval1mtab.at(2) = Htval1(_xi, -_x, 2., kHtuval);

        Htdval1mtab.at(0) = Htuval1mtab.at(0);
        Htdval1mtab.at(1) = Htuval1mtab.at(1);
        Htdval1mtab.at(2) = Htuval1mtab.at(2);

    } else {
        Htuval1tab.at(0) = Htval1_alt(_xi, _x, 0., kHtuval);
        Htuval1tab.at(1) = Htval1_alt(_xi, _x, 1., kHtuval);
        Htuval1tab.at(2) = Htval1_alt(_xi, _x, 2., kHtuval);

        Htdval1tab.at(0) = Htuval1tab.at(0);
        Htdval1tab.at(1) = Htuval1tab.at(1);
        Htdval1tab.at(2) = Htuval1tab.at(2);

        Htuval1mtab.at(0) = Htval1_alt(_xi, -_x, 0., kHtuval);
        Htuval1mtab.at(1) = Htval1_alt(_xi, -_x, 1., kHtuval);
        Htuval1mtab.at(2) = Htval1_alt(_xi, -_x, 2., kHtuval);

        Htdval1mtab.at(0) = Htuval1mtab.at(0);
        Htdval1mtab.at(1) = Htuval1mtab.at(1);
        Htdval1mtab.at(2) = Htuval1mtab.at(2);
    }
}

/*!
 * \fn void STGK11Model::calculateHtKas()
 *
 * Compute t-dependence of GPD Ht correlated to x.
 *
 */
void GK11Model::calculateHtKas(const double &_t) {
    double alpha, delta;

// gluons

    alpha = 0.15;  //slope of gluon trajectory
    delta = -0.78 + 0.17 * fL;
    kHtgluon = delta + alpha * _t;

// sea, not modelled by GK.

    kHtsea = 0.;

// u valence

    alpha = 0.45;
    delta = 0.48;
    kHtuval = delta + alpha * _t;

// d valence

    kHtdval = kHtuval;

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "(t=" << _t << ") kHtgluon=" << kHtgluon
                    << " kHtsea=" << kHtsea << " kHtuval=" << kHtuval
                    << " kHtdval=" << kHtdval);
}

/*!
 * \fn void STGK11Model::CalculateEtKas()
 *
 * Exactly the same as Hval1_alt and Eval1_alt.
 *
 */
double GK11Model::Htval1_alt(const double &_xi, double x, double i, double k) {
    return Hval1_alt(_xi, x, i, k);
}

/*!
 * \fn double STGK11Model::Hti1( double x, double i, double k )
 *
 * Exactly the same as Hs1.
 *
 */
double GK11Model::Hti1(const double &_xi, double x, double i, double k) {

    return Hs1(_xi, x, i, k);
}

/*!
 * \fn void STGK11Model::CalculateEtKas()
 *
 * Exactly the same as Hs1_alt.
 *
 */
double GK11Model::Hti1_alt(const double &_xi, double x, double i, double k) {

    //return Hs1_alt(_xi, x, i, k);

    cln::cl_R ia, ka, xba, xia;
    cln::cl_R dummya, dummy1, dummy2, dummy3;

    cln::float_format_t prec = cln::float_format(30);

    xia = cl_float(_xi, prec);
    ka = cl_float(k, prec);
    xba = cl_float(x, prec);
    ia = cl_float(i, prec);

    cln::cl_R xi5 = expt(xia, 5);

    dummy2 = exp((3 + ia - ka) * ln((xba + xia) / (1 + xia)));
    if (fabs(x - _xi) > 1e-6) {
        dummy1 = exp((3 + ia - ka) * ln((xba - xia) / (1 - xia)));
        dummy3 = exp((3 + ia - ka) * ln((-xba + xia) / (1 + xia)));
    } else {
        dummy1 = 0;
        dummy3 = 0;
    }

    double dummy;

    if ((x - _xi) < 0) {

        dummya = dummy2
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia - xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 - xba)
                                * (xia * xia - xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya += -dummy3
                * ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                        * expt(xia * xia + xba, 2)
                        + 6 * (3 + ia - ka) * xia * (1 + xba)
                                * (xia * xia + xba)
                        - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                                * (xba * xba - xia * xia));

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    } else {
        dummya = ((3 * expt(3 + ia - ka, 2) - (1 + ia - ka) * (5 + ia - ka))
                * expt(xia * xia - xba, 2)
                - 2 * (2 + ia - ka) * (4 + ia - ka) * (1 - xia * xia)
                        * (xba * xba - xia * xia)) * (dummy2 - dummy1)
                + 6 * (3 + ia - ka) * xia * (1 - xba) * (xia * xia - xba)
                        * (dummy2 + dummy1);

        dummya *= cl_float(15, prec) / 4 / xi5 / (1 + ia - ka) / (2 + ia - ka)
                / (3 + ia - ka) / (4 + ia - ka) / (5 + ia - ka);

    }
    dummy = double_approx(dummya);

    m_pLoggerManager->debug(getClassName(), __func__,
            Formatter() << "(x=" << x << ", xi=" << _xi << ", i=" << i << ", k="
                    << k << ") dummy = " << dummy);

    return dummy;
}

/*!
 * \fn void STGK11Model::CalculateEtKas()
 *
 * Exactly the same as Hval1 and Eval1.
 *
 */
double GK11Model::Htval1(const double &_xi, double x, double i, double k) {

    return Hval1(_xi, x, i, k);
}

/** !
 *  \fn void STGK11Model::calculateECoefs()
 *
 * Generic function for fast calculation.
 *
 * Calculates :
 *    - Es1  (i=0, 1, 2)
 *    - Ei1  (i=0, 1)
 *    - Eval1(i=0, 1, 2) (u)
 *    - Eval1(i=0, .. ,7) (d)
 *
 * Either fast (cln) or slow, depending on kinematic domain in (x,xi)
 * To be called before the evaluation of GPDs.
 *
 * Kinematic boundaries
 * For the sea and gluons : gluons share the same limits as strange/sea as far as slow/fast goes.
 *
 */
void GK11Model::calculateECoefs(const double &_x, const double &_xi,
        const double &_t) {
    int slow_sea = 0; // by default,  fast
    int slow_val = 0; // if  =  1 : slow ( full calculation with cln )

    if (log10(_xi) < (4. / 2.5) * log10(_x)) {
        slow_sea = 1;
    }

// For valence

    if ((_xi < 0.01) && (log10(_xi) < ((-4. / log10(0.6)) * log10(_x)))) {
        slow_val = 1;
    }

    calculateEKas(_t); // comes up with kEgluon,  kEsea,  kEuval,  kEdval

    if (!slow_sea) {
        Es1tab.at(0) = Es1(_xi, _x, 0., kEsea);
        Es1tab.at(1) = Es1(_xi, _x, 1., kEsea);
        Es1tab.at(2) = Es1(_xi, _x, 2., kEsea);

        Ei1tab.at(0) = Ei1(_xi, _x, 0., kEgluon);
        Ei1tab.at(1) = Ei1(_xi, _x, 1., kEgluon);
    } else {
        Es1tab.at(0) = Es1_alt(_xi, _x, 0., kEsea);
        Es1tab.at(1) = Es1_alt(_xi, _x, 1., kEsea);
        Es1tab.at(2) = Es1_alt(_xi, _x, 2., kEsea);

        Ei1tab.at(0) = Ei1_alt(_xi, _x, 0., kEgluon);
        Ei1tab.at(1) = Ei1_alt(_xi, _x, 1., kEgluon);
    }

    if (!slow_val) {
        Euval1tab.at(0) = Eval1(_xi, _x, 0., kEuval);
        Euval1tab.at(1) = Eval1(_xi, _x, 1., kEuval);

        Edval1tab.at(0) = Eval1(_xi, _x, 0., kEdval);
        Edval1tab.at(1) = Eval1(_xi, _x, 1., kEdval);
        Edval1tab.at(2) = Eval1(_xi, _x, 2., kEdval);
        Edval1tab.at(3) = Eval1(_xi, _x, 3., kEdval);
        Edval1tab.at(4) = Eval1(_xi, _x, 4., kEdval);
        Edval1tab.at(5) = Eval1(_xi, _x, 5., kEdval);
        Edval1tab.at(6) = Eval1(_xi, _x, 6., kEdval);
        Edval1tab.at(7) = Eval1(_xi, _x, 7., kEdval);

        Euval1mtab.at(0) = Eval1(_xi, -_x, 0., kEuval);
        Euval1mtab.at(1) = Eval1(_xi, -_x, 1., kEuval);

        Edval1mtab.at(0) = Eval1(_xi, -_x, 0., kEdval);
        Edval1mtab.at(1) = Eval1(_xi, -_x, 1., kEdval);
        Edval1mtab.at(2) = Eval1(_xi, -_x, 2., kEdval);
        Edval1mtab.at(3) = Eval1(_xi, -_x, 3., kEdval);
        Edval1mtab.at(4) = Eval1(_xi, -_x, 4., kEdval);
        Edval1mtab.at(5) = Eval1(_xi, -_x, 5., kEdval);
        Edval1mtab.at(6) = Eval1(_xi, -_x, 6., kEdval);
        Edval1mtab.at(7) = Eval1(_xi, -_x, 7., kEdval);

    } else {
        Euval1tab.at(0) = Eval1_alt(_xi, _x, 0., kEuval);
        Euval1tab.at(1) = Eval1_alt(_xi, _x, 1., kEuval);

        Edval1tab.at(0) = Eval1_alt(_xi, _x, 0., kEdval);
        Edval1tab.at(1) = Eval1_alt(_xi, _x, 1., kEdval);
        Edval1tab.at(2) = Eval1_alt(_xi, _x, 2., kEdval);
        Edval1tab.at(3) = Eval1_alt(_xi, _x, 3., kEdval);
        Edval1tab.at(4) = Eval1_alt(_xi, _x, 4., kEdval);
        Edval1tab.at(5) = Eval1_alt(_xi, _x, 5., kEdval);
        Edval1tab.at(6) = Eval1_alt(_xi, _x, 6., kEdval);
        Edval1tab.at(7) = Eval1_alt(_xi, _x, 7., kEdval);

        Euval1mtab.at(0) = Eval1_alt(_xi, -_x, 0., kEuval);
        Euval1mtab.at(1) = Eval1_alt(_xi, -_x, 1., kEuval);

        Edval1mtab.at(0) = Eval1_alt(_xi, -_x, 0., kEdval);
        Edval1mtab.at(1) = Eval1_alt(_xi, -_x, 1., kEdval);
        Edval1mtab.at(2) = Eval1_alt(_xi, -_x, 2., kEdval);
        Edval1mtab.at(3) = Eval1_alt(_xi, -_x, 3., kEdval);
        Edval1mtab.at(4) = Eval1_alt(_xi, -_x, 4., kEdval);
        Edval1mtab.at(5) = Eval1_alt(_xi, -_x, 5., kEdval);
        Edval1mtab.at(6) = Eval1_alt(_xi, -_x, 6., kEdval);
        Edval1mtab.at(7) = Eval1_alt(_xi, -_x, 7., kEdval);
    }

}

/*!
 * \fn void STGK11Model::calculateEKas()
 *
 * Compute t-dependence of GPD H correlated to x.
 *
 */
void GK11Model::calculateEKas(const double &_t) {
    double alpha, delta;

// gluons

    alpha = 0.15;  //slope of gluon trajectory
    delta = 0.10 + 0.06 * fL - 0.0027 * fL * fL;
    kEgluon = delta + alpha * _t;

// sea

    alpha = 0.15;  // same as slope of gluon trajectory
    delta = 1.1 + 0.06 * fL - 0.0027 * fL * fL;
    kEsea = delta + alpha * _t;

// u valence

    alpha = 0.9;
    delta = 0.48;
    kEuval = delta + alpha * _t;

// d valence

    alpha = 0.9;
    delta = 0.48;
    kEdval = delta + alpha * _t;

}

/*!
 * \fn double STGK11Model::Es1( double x, double i, double k )
 *
 * Exactly the same as Hs1.
 *
 */
double GK11Model::Es1(const double &_xi, double x, double i, double k) {
    return Hs1(_xi, x, i, k);
}

/*!
 * \fn double STGK11Model::Ei1( double x, double i, double k )
 *
 * Exactly the same as Hi1.
 *
 */
double GK11Model::Ei1(const double &_xi, double x, double i, double k) {
    return Hi1(_xi, x, i, k);
}

/*!
 * \fn double STGK11Model::Es1_alt( double x, double i, double k )
 *
 * Exactly the same as Hs1_alt.
 *
 */
double GK11Model::Es1_alt(const double &_xi, double x, double i, double k) {
    return Hs1_alt(_xi, x, i, k);
}

/*!
 * \fn double STGK11Model::Ei1_alt( double x, double i, double k )
 *
 * Exactly the same as Hi1_alt.
 *
 */
double GK11Model::Ei1_alt(const double &_xi, double x, double i, double k) {
    return Hi1_alt(_xi, x, i, k);
}

/*!
 * \fn double STGK11Model::Eval1( double x, double i, double k )
 *
 * Exactly the same as Hval1.
 *
 */
double GK11Model::Eval1(const double &_xi, double x, double i, double k) {
    return Hval1(_xi, x, i, k);
}

/*!
 * \fn void STGK11Model::CalculateEtKas()
 *
 * Exactly the same as Hval1_alt.
 *
 */
double GK11Model::Eval1_alt(const double &_xi, double x, double i, double k) {
    return Hval1_alt(_xi, x, i, k);
}

/** !
 *  \fn void STGK11Model::calculateEtCoefs()
 *
 * Generic function for fast calculation.
 *
 * Calculates :
 *    - Etval1  ( i = 0,  1,  2,  3 )
 *
 * Either fast (cln) or slow, depending on kinematic domain in (x,xi)
 * To be called before the evaluation of GPDs.
 *
 * Kinematic boundaries
 * For the sea and gluons : gluons share the same limits as strange/sea as far as slow/fast goes.
 *    For the sea ( and gluons later )
 *
 */
void GK11Model::calculateEtCoefs(const double &_x, const double &_xi,
        const double &_t) {
    int slow_val = 0; // if  =  1 : slow ( full calculation with cln )

// For valence

    if ((_xi < 0.04) && (log10(_xi) < ((-4. / log10(0.55)) * log10(_x)))) {
        slow_val = 1;
    }

    calculateEtKas(_t); // comes up with kHgluon,  kHsea,  kHuval,  kHdval

// WARNING : No sea or gluon Et for GK.

    if (!slow_val) {
        Etuval1tab.at(0) = Etval1(_xi, _x, 0., kEtuval);
        Etuval1tab.at(1) = Etval1(_xi, _x, 1., kEtuval);
        Etuval1tab.at(2) = Etval1(_xi, _x, 2., kEtuval);
        Etuval1tab.at(3) = Etval1(_xi, _x, 3., kEtuval);

        Etdval1tab.at(0) = Etuval1tab.at(0); // kEtdval & kEtuval are equal
        Etdval1tab.at(1) = Etuval1tab.at(1); // for u and d for Et,
        Etdval1tab.at(2) = Etuval1tab.at(2); // don't need to recalculate
        Etdval1tab.at(3) = Etuval1tab.at(3);

        Etuval1mtab.at(0) = Etval1(_xi, -_x, 0., kEtuval);
        Etuval1mtab.at(1) = Etval1(_xi, -_x, 1., kEtuval);
        Etuval1mtab.at(2) = Etval1(_xi, -_x, 2., kEtuval);
        Etuval1mtab.at(3) = Etval1(_xi, -_x, 3., kEtuval);

        Etdval1mtab.at(0) = Etuval1mtab.at(0);
        Etdval1mtab.at(1) = Etuval1mtab.at(1);
        Etdval1mtab.at(2) = Etuval1mtab.at(2);
        Etdval1mtab.at(3) = Etuval1mtab.at(3);

    } else {
        Etuval1tab.at(0) = Etval1_alt(_xi, _x, 0., kEtuval);
        Etuval1tab.at(1) = Etval1_alt(_xi, _x, 1., kEtuval);
        Etuval1tab.at(2) = Etval1_alt(_xi, _x, 2., kEtuval);
        Etuval1tab.at(3) = Etval1_alt(_xi, _x, 3., kEtuval);

        Etdval1tab.at(0) = Etuval1tab.at(0);
        Etdval1tab.at(1) = Etuval1tab.at(1);
        Etdval1tab.at(2) = Etuval1tab.at(2);
        Etdval1tab.at(3) = Etuval1tab.at(3);

        Etuval1mtab.at(0) = Etval1_alt(_xi, -_x, 0., kEtuval);
        Etuval1mtab.at(1) = Etval1_alt(_xi, -_x, 1., kEtuval);
        Etuval1mtab.at(2) = Etval1_alt(_xi, -_x, 2., kEtuval);
        Etuval1mtab.at(3) = Etval1_alt(_xi, -_x, 3., kEtuval);

        Etdval1mtab.at(0) = Etuval1mtab.at(0);
        Etdval1mtab.at(1) = Etuval1mtab.at(1);
        Etdval1mtab.at(2) = Etuval1mtab.at(2);
        Etdval1mtab.at(3) = Etuval1mtab.at(3);
    }
}

/*!
 * \fn void STGK11Model::calculateEtKas()
 *
 * Compute t-dependence of GPD Et correlated to x.
 *
 */
void GK11Model::calculateEtKas(const double &_t) {
    double alpha, delta;

// gluons, not modelled by GK.

    kEtgluon = 0.;

// sea, not modelled by GK.

    kEtsea = 0.;

// u valence

    alpha = 0.45;
    delta = 0.48;
    kEtuval = delta + alpha * _t;

// d valence

    kEtdval = kEtuval;
}

/* !
 *  \fn double STGK11Model::Et_pole( double x )
 *
 *  Pion pole contribution to Et.
 *
 */
double GK11Model::Et_pole(const double &_xi, const double &_t, double x) {

//TODO extraire les constantes si existantes

    double dum = 0.;  //  Initialize to 0
    double eps;
    double eps2;
    double tmin;
    double xbj;
    double Q2 = fMuF2;
    double tOverQ2 = _t / Q2;
    double y = (x + _xi) / 2. / _xi;
    double MPi2 = PI_ZERO_MASS * PI_ZERO_MASS;
    double gpiNN = 13.4;
    double f_pi = 0.131; // f_pi=0.131 GeV from paper. Actually 130.4 from PDG (Jul2010)
    double Lambda_N2 = 0.51 * 0.51;
    double Fp;
    double FpiNN;

    xbj = 2. * _xi / (_xi - tOverQ2 * _xi + 1. + tOverQ2 * 0.5);
    eps = 2. * xbj * PROTON_MASS / sqrt(Q2);
    eps2 = eps * eps;

    if (eps < 1 && (4. * xbj * (1. - xbj) + eps2) != 0) {

        tmin = -Q2 * (2. * (1. - xbj) * (1 - sqrt(1. - eps2)) + eps2)
                / (4. * xbj * (1. - xbj) + eps2);
        FpiNN = (Lambda_N2 - MPi2) / (Lambda_N2 - (_t - tmin));
        Fp = -PROTON_MASS * f_pi * (2. * sqrt(2.) * gpiNN * FpiNN)
                / (_t - MPi2);

        if (x < _xi && x > -_xi && _t < tmin) {
//          dum = ( Fp( pKinematicVariables->getT(), Q2, tmin ) / 4. / pGPDData->getXi() )  *  Phi_pi( ( x + pGPDData->getXi() ) / 2. / pGPDData->getXi() ) ;
            dum = (Fp / 4. / _xi) * 6. * y * (1. - y);
        }

    }

    return dum;
}

/*!
 * \fn double STGK11Model::Etval1( double x, double i, double k )
 *
 *  xdiff:=evalf(xb-xi); xsum:=evalf(xb+xi);
 *  if xsum<0 then    # xb<-xi
 *  Hi:=0 else
 *  if xdiff <0 then  # xb<xi
 *  Hi:= i -> 3/2/xi^3*(((xb+xi)/(1+xi))^(2+i-k)*(xi^2-xb+(2+i-k)*xi*(1-xb)))
 *            /(1+i-k)/(2+i-k)/(3+i-k)
 *  else
 *  Hi:= i -> 3/2/xi^3/(1+i-k)/(2+i-k)/(3+i-k)*((xi^2-xb)*( ((xb+xi)/(1+xi))^(2+i-k)
 *                                 -((xb-xi)/(1-xi))^(2+i-k))
 *                 + xi*(1-xb)*(2+i-k)*(((xb+xi)/(1+xi))^(2+i-k)+ ((xb-xi)/(1-xi))^(2+i-k) ) )
 *  end if: end if:
 *
 */
double GK11Model::Etval1(const double &_xi, double x, double i, double k) {
    double dum;

    if ((x + _xi) < 0) { // 0 for x < -pGPDData->getXi()
        dum = 0.0;
    } else { // Two cases for x >= -pGPDData->getXi()
        if ((x - _xi) < 0) {
            dum = 3. / 2. / pow(_xi, 3.)
                    * (pow((x + _xi) / (1. + _xi), (2. + i - k))
                            * (_xi * _xi - x + (2. + i - k) * _xi * (1. - x)))
                    / (1. + i - k) / (2. + i - k) / (3. + i - k);
        } else {
            dum = 3. / 2. / pow(_xi, 3.) / (1. + i - k) / (2. + i - k)
                    / (3. + i - k)
                    * ((_xi * _xi - x)
                            * (pow((x + _xi) / (1. + _xi), (2. + i - k))
                                    - pow((x - _xi) / (1. - _xi), (2. + i - k)))
                            + _xi * (1. - x) * (2. + i - k)
                                    * (pow((x + _xi) / (1. + _xi), (2. + i - k))
                                            + pow((x - _xi) / (1. - _xi),
                                                    (2. + i - k))));
        }
    }
    return dum;
}

/*!
 * \fn double STGK11Model::Etval1_alt( double x, double i, double k )
 *
 * Exactly the same as Hval1_alt and Eval1_alt, Htval1_alt.
 *
 */
double GK11Model::Etval1_alt(const double &_xi, double x, double i, double k) {
    return Hval1_alt(_xi, x, i, k);
}
