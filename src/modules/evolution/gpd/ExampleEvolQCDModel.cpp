#include "ExampleEvolQCDModel.h"

#include "../../../BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int ExampleEvolQCDModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new ExampleEvolQCDModel("ExampleEvolQCDModel"));

ExampleEvolQCDModel::ExampleEvolQCDModel(const std::string &className) :
        GPDEvolutionModule(className) {
}

ExampleEvolQCDModel::ExampleEvolQCDModel(const ExampleEvolQCDModel& other) :
        GPDEvolutionModule(other) {
}

ExampleEvolQCDModel::~ExampleEvolQCDModel() {
}

ExampleEvolQCDModel* ExampleEvolQCDModel::clone() const {
    return new ExampleEvolQCDModel(*this);
}

void ExampleEvolQCDModel::isModuleWellConfigured() {
    GPDEvolutionModule::isModuleWellConfigured();
}

void ExampleEvolQCDModel::initModule() {
    GPDEvolutionModule::initModule();
}

double ExampleEvolQCDModel::integratedNonSingletMuFDerivative(
        const NfInterval& nfInterval) {
    return 1.;
}

double ExampleEvolQCDModel::integratedSingletMuFDerivative(const NfInterval& nfInterval) {
    return 1.;
}

double ExampleEvolQCDModel::integratedGluonMuFDerivative(const NfInterval& nfInterval) {
    return 1.;
}
