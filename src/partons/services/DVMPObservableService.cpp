#include "../../../include/partons/services/DVMPObservableService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/modules/observable/DVMP/DVMPObservable.h"
#include "../../../include/partons/modules/process/DVMP/DVMPProcessModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int DVMPObservableService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DVMPObservableService("DVMPObservableService"));

DVMPObservableService::DVMPObservableService(const std::string &className) :
        ObservableService(className) {
}

DVMPObservableService::~DVMPObservableService() {
}

void DVMPObservableService::resolveObjectDependencies() {
    ObservableService<DVMPObservableKinematic, DVMPObservableResult>::resolveObjectDependencies();
}

DVMPObservableKinematic DVMPObservableService::newKinematicFromTask(
        const Task& task) const {

    //create a kinematic and init it with a list of parameters
    DVMPObservableKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVMPObservableKinematic::DVMP_OBSERVABLE_KNEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<DVMPObservableKinematic> DVMPObservableService::newListOfKinematicFromTask(
        const Task& task) const {

    List<DVMPObservableKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVMPObservableKinematic::DVMP_OBSERVABLE_KNEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getDVMPObservableKinematicFromFile(
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

DVMPProcessModule* DVMPObservableService::newDVMPProcessModuleFromTask(
        const Task &task) const {

    //initialize
    DVMPProcessModule* pProcessModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DVMPProcessModule::DVMP_PROCESS_MODULE_CLASS_NAME)) {

        //configure
        pProcessModule =
                Partons::getInstance()->getModuleObjectFactory()->newDVMPProcessModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pProcessModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pProcessModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DVMPProcessModule::DVMP_PROCESS_MODULE_CLASS_NAME);
    }

    //return
    return pProcessModule;

}

DVMPObservable* DVMPObservableService::newDVMPObservableModuleFromTask(
        const Task &task) const {

    //initialize
    DVMPObservable* pObservable = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DVMPObservable::DVMP_OBSERVABLE_MODULE_CLASS_NAME)) {

        //configure
        pObservable =
                Partons::getInstance()->getModuleObjectFactory()->newDVMPObservable(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pObservable->configure(
                task.getModuleComputationConfiguration().getParameters());

        pObservable->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DVMPObservable::DVMP_OBSERVABLE_MODULE_CLASS_NAME);
    }

    //return
    return pObservable;
}

ProcessModule<DVMPObservableKinematic, DVMPObservableResult>* DVMPObservableService::newProcessModuleFromTask(
        const Task &task) const {
    return newDVMPProcessModuleFromTask(task);
}

Observable<DVMPObservableKinematic, DVMPObservableResult>* DVMPObservableService::newObservableModuleFromTask(
        const Task &task) const {
    return newDVMPObservableModuleFromTask(task);
}

} /* namespace PARTONS */
