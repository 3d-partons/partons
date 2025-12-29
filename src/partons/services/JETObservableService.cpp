#include "../../../include/partons/services/JETObservableService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/modules/observable/JET/JETObservable.h"
#include "../../../include/partons/modules/process/JET/JETProcessModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int JETObservableService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new JETObservableService("JETObservableService"));

JETObservableService::JETObservableService(const std::string &className) :
        ObservableService(className) {
}

JETObservableService::~JETObservableService() {
}

void JETObservableService::resolveObjectDependencies() {
    ObservableService<JETObservableKinematic, JETObservableResult>::resolveObjectDependencies();
}

JETObservableKinematic JETObservableService::newKinematicFromTask(
        const Task& task) const {

    //create a kinematic and init it with a list of parameters
    JETObservableKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            JETObservableKinematic::JET_OBSERVABLE_KINEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<JETObservableKinematic> JETObservableService::newListOfKinematicFromTask(
        const Task& task) const {

    List<JETObservableKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            JETObservableKinematic::JET_OBSERVABLE_KINEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getJETObservableKinematicFromFile(
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

JETProcessModule* JETObservableService::newJETProcessModuleFromTask(
        const Task &task) const {

    //initialize
    JETProcessModule* pProcessModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            JETProcessModule::JET_PROCESS_MODULE_CLASS_NAME)) {

        //configure
        pProcessModule =
                Partons::getInstance()->getModuleObjectFactory()->newJETProcessModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pProcessModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pProcessModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << JETProcessModule::JET_PROCESS_MODULE_CLASS_NAME);
    }

    //return
    return pProcessModule;

}

JETObservable* JETObservableService::newJETObservableModuleFromTask(
        const Task &task) const {

    //initialize
    JETObservable* pObservable = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            JETObservable::JET_OBSERVABLE_MODULE_CLASS_NAME)) {

        //configure
        pObservable =
                Partons::getInstance()->getModuleObjectFactory()->newJETObservable(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pObservable->configure(
                task.getModuleComputationConfiguration().getParameters());

        pObservable->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << JETObservable::JET_OBSERVABLE_MODULE_CLASS_NAME);
    }

    //return
    return pObservable;
}

ProcessModule<JETObservableKinematic, JETObservableResult>* JETObservableService::newProcessModuleFromTask(
        const Task &task) const {
    return newJETProcessModuleFromTask(task);
}

Observable<JETObservableKinematic, JETObservableResult>* JETObservableService::newObservableModuleFromTask(
        const Task &task) const {
    return newJETObservableModuleFromTask(task);
}

} /* namespace PARTONS */
