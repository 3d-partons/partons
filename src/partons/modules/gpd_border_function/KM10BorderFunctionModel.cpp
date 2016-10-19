/*
 * KM10BorderFunctionModel.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#include "../../../../include/partons/modules/gpd_border_function/KM10BorderFunctionModel.h"

#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

const unsigned int KM10BorderFunctionModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new KM10BorderFunctionModel("KM10BorderFunctionModel"));

KM10BorderFunctionModel::KM10BorderFunctionModel(const std::string& className) :
        GPDBorderFunctionModule(className) {

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
}

KM10BorderFunctionModel::~KM10BorderFunctionModel() {
}

KM10BorderFunctionModel* KM10BorderFunctionModel::clone() const {
    return new KM10BorderFunctionModel(*this);
}

void KM10BorderFunctionModel::resolveObjectDependencies() {
}

void KM10BorderFunctionModel::configure(const ElemUtils::Parameters& parameters) {
    GPDBorderFunctionModule::configure(parameters);
}

std::string KM10BorderFunctionModel::toString() {
    GPDBorderFunctionModule::toString();
}

PartonDistribution KM10BorderFunctionModel::computeH() {

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //GPDs for x
    uVal = 1.;
    dVal = 2.;
    uSea = 3.;
    dSea = 4.;
    sSea = 5.;
    g = 6.;

    //GPDs for -x

    uValMx = 0.;
    dValMx = 0.;

    //store results
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * sSea);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}
