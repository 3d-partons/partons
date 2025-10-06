#include "../../../include/partons/services/TCSConvolCoeffFunctionService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/modules/convol_coeff_function/TCS/TCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int TCSConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new TCSConvolCoeffFunctionService(
                        "TCSConvolCoeffFunctionService"));

TCSConvolCoeffFunctionService::TCSConvolCoeffFunctionService(
        const std::string &className) :
        ConvolCoeffFunctionService<TCSConvolCoeffFunctionKinematic,
                TCSConvolCoeffFunctionResult>(className) {
}

TCSConvolCoeffFunctionService::~TCSConvolCoeffFunctionService() {
}

void TCSConvolCoeffFunctionService::resolveObjectDependencies() {
    ConvolCoeffFunctionService<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>::resolveObjectDependencies();
}

TCSConvolCoeffFunctionKinematic TCSConvolCoeffFunctionService::newKinematicFromTask(
        const Task& task) const {
    TCSConvolCoeffFunctionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            TCSConvolCoeffFunctionKinematic::TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <TCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<TCSConvolCoeffFunctionKinematic> TCSConvolCoeffFunctionService::newListOfKinematicFromTask(
        const Task& task) const {
    List<TCSConvolCoeffFunctionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            TCSConvolCoeffFunctionKinematic::TCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getTCSCCFKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <TCSConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <TCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

TCSConvolCoeffFunctionModule* TCSConvolCoeffFunctionService::newTCSConvolCoeffFunctionModuleFromTask(
        const Task &task) const {

    //initialize
    TCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            TCSConvolCoeffFunctionModule::TCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME)) {

        //configure
        pConvolCoeffFunctionModule =
                Partons::getInstance()->getModuleObjectFactory()->newTCSConvolCoeffFunctionModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pConvolCoeffFunctionModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pConvolCoeffFunctionModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << TCSConvolCoeffFunctionModule::TCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);
    }

    //return
    return pConvolCoeffFunctionModule;
}

ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
        TCSConvolCoeffFunctionResult>* TCSConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
        const Task &task) const {
    return newTCSConvolCoeffFunctionModuleFromTask(task);
}

} /* namespace PARTONS */
