#include "../../../include/partons/services/DDVCSObservableService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/observable/service/DDVCSObservableResultDaoService.h"
#include "../../../include/partons/modules/observable/DDVCS/DDVCSObservable.h"
#include "../../../include/partons/modules/process/DDVCS/DDVCSProcessModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int DDVCSObservableService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new DDVCSObservableService("DDVCSObservableService"));

DDVCSObservableService::DDVCSObservableService(const std::string &className) :
        ObservableService(className) {
}

DDVCSObservableService::~DDVCSObservableService() {
}

void DDVCSObservableService::resolveObjectDependencies() {
    ObservableService<DDVCSObservableKinematic, DDVCSObservableResult>::resolveObjectDependencies();
}

DDVCSObservableKinematic DDVCSObservableService::newKinematicFromTask(
        const Task& task) const {

    //create a kinematic and init it with a list of parameters
    DDVCSObservableKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DDVCSObservableKinematic::DDVCS_OBSERVABLE_KINEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<DDVCSObservableKinematic> DDVCSObservableService::newListOfKinematicFromTask(
        const Task& task) const {

    List<DDVCSObservableKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DDVCSObservableKinematic::DDVCS_OBSERVABLE_KINEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getDDVCSObservableKinematicFromFile(
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

void DDVCSObservableService::storeResultListInDatabase(
        const List<DDVCSObservableResult>& results) const {

    //get dao service
    DDVCSObservableResultDaoService ddvcsObservableResultDaoService;

    //insert
    int computationId = ddvcsObservableResultDaoService.insert(results);

    //check if inserted correctly
    if (computationId != -1) {
        info(__func__,
                ElemUtils::Formatter()
                        << "List of DDVCSObservableResult objects has been stored in database with computation_id = "
                        << computationId);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "DDVCSObservableResult object : insertion into database failed");
    }
}

void DDVCSObservableService::generatePlotFileTask(Task &task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task,
                    "ddvcs_observable_plot_2d_view"), ' ');
}

DDVCSProcessModule* DDVCSObservableService::newDDVCSProcessModuleFromTask(
        const Task &task) const {

    //initialize
    DDVCSProcessModule* pProcessModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DDVCSProcessModule::DDVCS_PROCESS_MODULE_CLASS_NAME)) {

        //configure
        pProcessModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSProcessModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pProcessModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pProcessModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DDVCSProcessModule::DDVCS_PROCESS_MODULE_CLASS_NAME);
    }

    //return
    return pProcessModule;

}

DDVCSObservable* DDVCSObservableService::newDDVCSObservableModuleFromTask(
        const Task &task) const {

    //initialize
    DDVCSObservable* pObservable = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DDVCSObservable::DDVCS_OBSERVABLE_MODULE_CLASS_NAME)) {

        //configure
        pObservable =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSObservable(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pObservable->configure(
                task.getModuleComputationConfiguration().getParameters());

        pObservable->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << DDVCSObservable::DDVCS_OBSERVABLE_MODULE_CLASS_NAME);
    }

    //return
    return pObservable;
}

ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>* DDVCSObservableService::newProcessModuleFromTask(
        const Task &task) const {
    return newDDVCSProcessModuleFromTask(task);
}

Observable<DDVCSObservableKinematic, DDVCSObservableResult>* DDVCSObservableService::newObservableModuleFromTask(
        const Task &task) const {
    return newDDVCSObservableModuleFromTask(task);
}

} /* namespace PARTONS */
