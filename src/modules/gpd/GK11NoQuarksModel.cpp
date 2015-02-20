/*
 * GK11NoQuarksModel.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: kuba
 */

#include <cmath>
#include "GK11NoQuarksModel.h"
#include "../../services/ModuleObjectFactory.h"
#include "../../utils/logger/LoggerManager.h"
#include "../../utils/stringUtils/Formatter.h"


// Initialise GK11NoQuarksModule::moduleID with a unique name and enable registerModule() to be executed before the main() function.
const std::string GK11NoQuarksModel::moduleID =
        ModuleObjectFactory::getInstance()->registerModule(new GK11NoQuarksModel("GK11NoQuarksModel"));


GK11NoQuarksModel::GK11NoQuarksModel(const std::string& className)  : GK11Model(className){
}

GK11NoQuarksModel::~GK11NoQuarksModel() {
	// TODO Auto-generated destructor stub
}

GK11NoQuarksModel* GK11NoQuarksModel::clone() const {
	return new GK11NoQuarksModel(*this);
}

GK11NoQuarksModel::GK11NoQuarksModel(const GK11NoQuarksModel& other): GK11Model(other) {
}

GPDResultData GK11NoQuarksModel::computeH() {
	GPDComputeType gpdComputeType(GPDComputeType::H);

	GPDResultData GPD_H(gpdComputeType);

	GPDQuarkFlavorData gpdQuarkFlavorData_u(gpdComputeType, QuarkFlavor::UP);
	GPDQuarkFlavorData gpdQuarkFlavorData_d(gpdComputeType, QuarkFlavor::DOWN);
	GPDQuarkFlavorData gpdQuarkFlavorData_s(gpdComputeType, QuarkFlavor::STRANGE);

    calculateHCoefs(); // Calculate the K's and the coefficients

// Gluons

    c1 = 2.23 + 0.362 * fL; // See table 1 p. 12
    c2 = 5.43 - 7.0 * fL; // See table 1 p. 12
    c3 = -34.0 + 22.5 * fL; // See table 1 p. 12
    c4 = 40.6 - 21.6 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + fMuF2)); // See eq. (39) p. 14

    GPD_H.setGluon(
            exp(b0 * m_t)
                    * (c1 * Hi1tab.at(0) + c2 * Hi1tab.at(1) + c3 * Hi1tab.at(2)
                            + c4 * Hi1tab.at(3))); // See eq. (27)

/*    m_pLoggerManager->debug(getClassName(), __func__,
                Formatter() << "1st coef " << Hi1tab.at(0)
                			<< "2nd coef " << Hi1tab.at(1)
                			<< "3rd coef " << Hi1tab.at(2)
                			<< "4rdt coef " << Hi1tab.at(3)) ;*/


// quarks set to zero
    gpdQuarkFlavorData_s.setPartonDistribution(0.0);
    gpdQuarkFlavorData_u.setPartonDistribution(0.0);
    gpdQuarkFlavorData_d.setPartonDistribution(0.0);

    gpdQuarkFlavorData_u.setValence(0.0);
    gpdQuarkFlavorData_d.setValence(0.0);
    gpdQuarkFlavorData_s.setValence(0.0);

    gpdQuarkFlavorData_u.setSea(0.0);
    gpdQuarkFlavorData_d.setSea(0.0);
    gpdQuarkFlavorData_s.setSea(0.0);

    gpdQuarkFlavorData_u.setPartonDistributionSinglet(0.0);

        gpdQuarkFlavorData_d.setPartonDistributionSinglet(0.0);

        gpdQuarkFlavorData_s.setPartonDistributionSinglet(0.0);

        GPD_H.setSinglet(0.0);

        // Set Hq(+), Hq(-)
        // u quark
        gpdQuarkFlavorData_u.setPartonDistributionPlus(0.0);

        gpdQuarkFlavorData_u.setPartonDistributionMinus(0.0);

        // d quark
        gpdQuarkFlavorData_d.setPartonDistributionPlus(0.0);

        gpdQuarkFlavorData_d.setPartonDistributionMinus(0.0);

        // s quark
        gpdQuarkFlavorData_s.setPartonDistributionPlus(0.);

        GPD_H.addGPDQuarkFlavorData(gpdQuarkFlavorData_u);
        GPD_H.addGPDQuarkFlavorData(gpdQuarkFlavorData_d);
        GPD_H.addGPDQuarkFlavorData(gpdQuarkFlavorData_s);

	return GPD_H;
}

GPDResultData GK11NoQuarksModel::computeE() {
	return GPDResultData();
}

GPDResultData GK11NoQuarksModel::computeHt() {
	return GPDResultData();
}

GPDResultData GK11NoQuarksModel::computeEt() {
	return GPDResultData();
}
