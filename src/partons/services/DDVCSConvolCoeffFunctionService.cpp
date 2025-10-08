#include "../../../include/partons/services/DDVCSConvolCoeffFunctionService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/modules/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int DDVCSConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DDVCSConvolCoeffFunctionService(
                        "DDVCSConvolCoeffFunctionService"));

DDVCSConvolCoeffFunctionService::DDVCSConvolCoeffFunctionService(
        const std::string &className) :
        ConvolCoeffFunctionService<DDVCSConvolCoeffFunctionKinematic,
                DDVCSConvolCoeffFunctionResult>(className) {
}

DDVCSConvolCoeffFunctionService::~DDVCSConvolCoeffFunctionService() {
}

void DDVCSConvolCoeffFunctionService::resolveObjectDependencies() {
    ConvolCoeffFunctionService<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>::resolveObjectDependencies();
}

DDVCSConvolCoeffFunctionKinematic DDVCSConvolCoeffFunctionService::newKinematicFromTask(
        const Task& task) const {
    DDVCSConvolCoeffFunctionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DDVCSConvolCoeffFunctionKinematic::DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DDVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<DDVCSConvolCoeffFunctionKinematic> DDVCSConvolCoeffFunctionService::newListOfKinematicFromTask(
        const Task& task) const {
    List<DDVCSConvolCoeffFunctionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DDVCSConvolCoeffFunctionKinematic::DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getDDVCSCCFKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <DDVCSConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DDVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

DDVCSConvolCoeffFunctionModule* DDVCSConvolCoeffFunctionService::newDDVCSConvolCoeffFunctionModuleFromTask(
        const Task &task) const {

    //initialize
    DDVCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DDVCSConvolCoeffFunctionModule::DDVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME)) {

        //configure
        pConvolCoeffFunctionModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSConvolCoeffFunctionModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pConvolCoeffFunctionModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pConvolCoeffFunctionModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DDVCSConvolCoeffFunctionModule::DDVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME);
    }

    //return
    return pConvolCoeffFunctionModule;
}

ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
        DDVCSConvolCoeffFunctionResult>* DDVCSConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
        const Task &task) const {
    return newDDVCSConvolCoeffFunctionModuleFromTask(task);

}

} /* namespace PARTONS */
