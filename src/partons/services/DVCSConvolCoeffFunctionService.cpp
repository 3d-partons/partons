#include "../../../include/partons/services/DVCSConvolCoeffFunctionService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int DVCSConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DVCSConvolCoeffFunctionService(
                        "DVCSConvolCoeffFunctionService"));

DVCSConvolCoeffFunctionService::DVCSConvolCoeffFunctionService(
        const std::string &className) :
        ConvolCoeffFunctionService<DVCSConvolCoeffFunctionKinematic,
                DVCSConvolCoeffFunctionResult>(className) {
}

DVCSConvolCoeffFunctionService::~DVCSConvolCoeffFunctionService() {
}

void DVCSConvolCoeffFunctionService::resolveObjectDependencies() {
    ConvolCoeffFunctionService<DVCSConvolCoeffFunctionKinematic,
            DVCSConvolCoeffFunctionResult>::resolveObjectDependencies();
}

//TODO remove hardcoded string
DVCSConvolCoeffFunctionKinematic DVCSConvolCoeffFunctionService::newKinematicFromTask(
        const Task& task) const {
    DVCSConvolCoeffFunctionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            "DVCSConvolCoeffFunctionKinematic")) {
        kinematic = DVCSConvolCoeffFunctionKinematic(
                task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

//TODO remove hardcoded string
List<DVCSConvolCoeffFunctionKinematic> DVCSConvolCoeffFunctionService::newListOfKinematicFromTask(
        const Task& task) const {
    List<DVCSConvolCoeffFunctionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            "DVCSConvolCoeffFunctionKinematic")) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getCCFKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <DVCSConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

} /* namespace PARTONS */
