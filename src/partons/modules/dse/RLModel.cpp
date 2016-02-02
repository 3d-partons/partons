#include "../../../../include/partons/modules/dse/RLModel.h"

#include "../../../../include/partons/BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int RLModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new RLModel("RLModel"));

RLModel::RLModel(const std::string &className)
        : GapEquationSolverModule(className) {

}
RLModel::~RLModel() {
    // TODO Auto-generated destructor stub
}

RLModel::RLModel(const RLModel& other)
        : GapEquationSolverModule(other) {
}

RLModel* RLModel::clone() const {
    return new RLModel(*this);
}

void RLModel::initModule() {
    GapEquationSolverModule::initModule();
}

void RLModel::isModuleWellConfigured() {
    GapEquationSolverModule::isModuleWellConfigured();
}
