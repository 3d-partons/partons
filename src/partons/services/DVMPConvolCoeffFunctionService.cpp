#include "../../../include/partons/services/DVMPConvolCoeffFunctionService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/modules/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int DVMPConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DVMPConvolCoeffFunctionService(
                        "DVMPConvolCoeffFunctionService"));

DVMPConvolCoeffFunctionService::DVMPConvolCoeffFunctionService(
        const std::string &className) :
        ConvolCoeffFunctionService<DVMPConvolCoeffFunctionKinematic,
                DVMPConvolCoeffFunctionResult>(className) {
}

DVMPConvolCoeffFunctionService::~DVMPConvolCoeffFunctionService() {
}

void DVMPConvolCoeffFunctionService::resolveObjectDependencies() {
    ConvolCoeffFunctionService<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>::resolveObjectDependencies();
}

DVMPConvolCoeffFunctionKinematic DVMPConvolCoeffFunctionService::newKinematicFromTask(
        const Task& task) const {
    DVMPConvolCoeffFunctionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVMPConvolCoeffFunctionKinematic::DVMP_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVMPConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<DVMPConvolCoeffFunctionKinematic> DVMPConvolCoeffFunctionService::newListOfKinematicFromTask(
        const Task& task) const {
    List<DVMPConvolCoeffFunctionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVMPConvolCoeffFunctionKinematic::DVMP_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getDVMPCCFKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <DVMPConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVMPConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

DVMPConvolCoeffFunctionModule* DVMPConvolCoeffFunctionService::newDVMPConvolCoeffFunctionModuleFromTask(
        const Task &task) const {

    //initialize
    DVMPConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DVMPConvolCoeffFunctionModule::DVMP_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME)) {

        //configure
        pConvolCoeffFunctionModule =
                Partons::getInstance()->getModuleObjectFactory()->newDVMPConvolCoeffFunctionModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pConvolCoeffFunctionModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pConvolCoeffFunctionModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DVMPConvolCoeffFunctionModule::DVMP_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);
    }

    //return
    return pConvolCoeffFunctionModule;
}

ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
        DVMPConvolCoeffFunctionResult>* DVMPConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
        const Task &task) const {
    return newDVMPConvolCoeffFunctionModuleFromTask(task);

}

} /* namespace PARTONS */
