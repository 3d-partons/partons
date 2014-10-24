#include "VinnikovEvolQCDModel.h"

#include "../../services/ModuleObjectFactory.h"

// Initialise GK11GPDModule::moduleID with a unique name.
const std::string VinnikovEvolQCDModel::moduleID = "VinnikovEvolQCDModel";

// Define a useless static boolean variable to enable registerModule() to be executed before the main() function.
// Because global variables have program scope, and are initialised before main() is called.
// !!! CARE !!! variable name must be unique.
static bool isVinnikovEvolQCDRegistered =
		ModuleObjectFactory::getInstance()->registerModule(
				new VinnikovEvolQCDModel());

VinnikovEvolQCDModel::VinnikovEvolQCDModel() :
		EvolQCDModule(VinnikovEvolQCDModel::moduleID) {

}

VinnikovEvolQCDModel::~VinnikovEvolQCDModel() {

}

VinnikovEvolQCDModel::VinnikovEvolQCDModel(const VinnikovEvolQCDModel &other) :
		EvolQCDModule(other) {

}

VinnikovEvolQCDModel* VinnikovEvolQCDModel::clone() const {
	return new VinnikovEvolQCDModel(*this);
}

//TODO implementer
void VinnikovEvolQCDModel::isModuleWellConfigured() {
}

void VinnikovEvolQCDModel::initModule() {

}

GPDResultData VinnikovEvolQCDModel::compute(const double &x, const double &xi,
		const double &t, const double &MuF, const double &MuR,
		const GPDResultData &gpdResultData) {
	EvolQCDModule::preCompute(x, xi, t, MuF, MuR, gpdResultData);

	EvolQCDModule::convertBasis();

	evolution();

	EvolQCDModule::invertBasis();

	return EvolQCDModule::makeGPDResultData();
}

void VinnikovEvolQCDModel::evolution() {

}
