#include "../../../include/partons/services/JETConvolCoeffFunctionService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/modules/convol_coeff_function/JET/JETConvolCoeffFunctionModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int JETConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new JETConvolCoeffFunctionService(
                        "JETConvolCoeffFunctionService"));

JETConvolCoeffFunctionService::JETConvolCoeffFunctionService(
        const std::string &className) :
        ConvolCoeffFunctionService<JETConvolCoeffFunctionKinematic,
                JETConvolCoeffFunctionResult>(className) {
}

JETConvolCoeffFunctionService::~JETConvolCoeffFunctionService() {
}

void JETConvolCoeffFunctionService::resolveObjectDependencies() {
    ConvolCoeffFunctionService<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>::resolveObjectDependencies();
}

JETConvolCoeffFunctionKinematic JETConvolCoeffFunctionService::newKinematicFromTask(
        const Task& task) const {
    JETConvolCoeffFunctionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            JETConvolCoeffFunctionKinematic::JET_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <JETConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<JETConvolCoeffFunctionKinematic> JETConvolCoeffFunctionService::newListOfKinematicFromTask(
        const Task& task) const {
    List<JETConvolCoeffFunctionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            JETConvolCoeffFunctionKinematic::JET_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getJETCCFKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <JETConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <JETConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

JETConvolCoeffFunctionModule* JETConvolCoeffFunctionService::newJETConvolCoeffFunctionModuleFromTask(
        const Task &task) const {

    //initialize
    JETConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            JETConvolCoeffFunctionModule::JET_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME)) {

        //configure
        pConvolCoeffFunctionModule =
                Partons::getInstance()->getModuleObjectFactory()->newJETConvolCoeffFunctionModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pConvolCoeffFunctionModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pConvolCoeffFunctionModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << JETConvolCoeffFunctionModule::JET_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);
    }

    //return
    return pConvolCoeffFunctionModule;
}

ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
        JETConvolCoeffFunctionResult>* JETConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
        const Task &task) const {
    return newJETConvolCoeffFunctionModuleFromTask(task);

}

} /* namespace PARTONS */
