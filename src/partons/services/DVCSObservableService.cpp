#include "../../../include/partons/services/DVCSObservableService.h"

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

const unsigned int DVCSObservableService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DVCSObservableService("DVCSObservableService"));

DVCSObservableService::DVCSObservableService(const std::string &className) :
        ObservableService(className) {
}

DVCSObservableService::~DVCSObservableService() {
}

void DVCSObservableService::resolveObjectDependencies() {
    ObservableService<DVCSObservableKinematic, DVCSObservableResult>::resolveObjectDependencies();
}

//TODO remove hardcoded string
DVCSObservableKinematic DVCSObservableService::newKinematicFromTask(
        const Task& task) const {

    //create a kinematic and init it with a list of parameters
    DVCSObservableKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            "ObservableKinematic")) {
        kinematic = DVCSObservableKinematic(
                task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

//TODO remove hardcoded string
List<DVCSObservableKinematic> DVCSObservableService::newListOfKinematicFromTask(
        const Task& task) const {

    List<DVCSObservableKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            "ObservableKinematic")) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getDVCSObservableKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <ObservableKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

} /* namespace PARTONS */
