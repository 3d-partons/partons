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
#include "../../../include/partons/database/observable/service/DVCSObservableResultDaoService.h"
#include "../../../include/partons/modules/observable/DVCS/DVCSObservable.h"
#include "../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
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

DVCSObservableKinematic DVCSObservableService::newKinematicFromTask(
        const Task& task) const {

    //create a kinematic and init it with a list of parameters
    DVCSObservableKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVCSObservableKinematic::DVCS_OBSERVABLE_KNEMATIC_CLASS_NAME)) {
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

List<DVCSObservableKinematic> DVCSObservableService::newListOfKinematicFromTask(
        const Task& task) const {

    List<DVCSObservableKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVCSObservableKinematic::DVCS_OBSERVABLE_KNEMATIC_CLASS_NAME)) {

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

void DVCSObservableService::storeResultListInDatabase(
        const List<DVCSObservableResult>& results) const {

    //get dao service
    DVCSObservableResultDaoService dvcsObservableResultDaoService;

    //insert
    int computationId = dvcsObservableResultDaoService.insert(results);

    //check if inserted correctly
    if (computationId != -1) {
        info(__func__,
                ElemUtils::Formatter()
                        << "DVCSObservableResult object has been stored in database with computation_id = "
                        << computationId);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "DVCSObservableResult object : insertion into database failed");
    }
}

void DVCSObservableService::generatePlotFileTask(Task &task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task,
                    "dvcs_observable_plot_2d_view"), ' ');
}

ProcessModule<DVCSObservableKinematic, DVCSObservableResult>* DVCSObservableService::newProcessModuleFromTask(
        const Task &task) const {

    //initialize
    ProcessModule<DVCSObservableKinematic, DVCSObservableResult>* pProcessModule =
            0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DVCSProcessModule::DVCS_PROCESS_MODULE_CLASS_NAME)) {

        //configure
        pProcessModule =
                Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pProcessModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pProcessModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "You have not provided any ProcessModule ; Or check case in your XML file");
    }

    //return
    return pProcessModule;
}

Observable<DVCSObservableKinematic, DVCSObservableResult>* DVCSObservableService::newObservableModuleFromTask(
        const Task &task) const {

    //initialize
    Observable<DVCSObservableKinematic, DVCSObservableResult>* pObservable = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DVCSObservable::DVCS_OBSERVABLE_MODULE_CLASS_NAME)) {

        //configure
        pObservable =
                Partons::getInstance()->getModuleObjectFactory()->newDVCSObservable(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pObservable->configure(
                task.getModuleComputationConfiguration().getParameters());

        pObservable->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "You have not provided any Observable");
    }

    //return
    return pObservable;
}

} /* namespace PARTONS */
