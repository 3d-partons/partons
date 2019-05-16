#include "../../../include/partons/services/TCSObservableService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/observable/service/TCSObservableResultDaoService.h"
#include "../../../include/partons/modules/observable/TCS/TCSObservable.h"
#include "../../../include/partons/modules/process/TCS/TCSProcessModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {

const unsigned int TCSObservableService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new TCSObservableService("TCSObservableService"));

TCSObservableService::TCSObservableService(const std::string &className) :
        ObservableService(className) {
}

TCSObservableService::~TCSObservableService() {
}

void TCSObservableService::resolveObjectDependencies() {
    ObservableService<TCSObservableKinematic, TCSObservableResult>::resolveObjectDependencies();
}

TCSObservableKinematic TCSObservableService::newKinematicFromTask(
        const Task& task) const {

    //create a kinematic and init it with a list of parameters
    TCSObservableKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            TCSObservableKinematic::TCS_OBSERVABLE_KNEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <ObservableKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<TCSObservableKinematic> TCSObservableService::newListOfKinematicFromTask(
        const Task& task) const {

    List<TCSObservableKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            TCSObservableKinematic::TCS_OBSERVABLE_KNEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getTCSObservableKinematicFromFile(
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

void TCSObservableService::storeResultListInDatabase(
        const List<TCSObservableResult>& results) const {

    //get dao service
    TCSObservableResultDaoService tcsObservableResultDaoService;

    //insert
    int computationId = tcsObservableResultDaoService.insert(results);

    //check if inserted correctly
    if (computationId != -1) {
        info(__func__,
                ElemUtils::Formatter()
                        << "TCSObservableResult object has been stored in database with computation_id = "
                        << computationId);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "TCSObservableResult object : insertion into database failed");
    }
}

void TCSObservableService::generatePlotFileTask(Task &task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task,
                    "tcs_observable_plot_2d_view"), ' ');
}

ProcessModule<TCSObservableKinematic, TCSObservableResult>* TCSObservableService::newProcessModuleFromTask(
        const Task &task) const {

    //initialize
    ProcessModule<TCSObservableKinematic, TCSObservableResult>* pProcessModule =
            0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            TCSProcessModule::TCS_PROCESS_MODULE_CLASS_NAME)) {

        //configure
        pProcessModule =
                Partons::getInstance()->getModuleObjectFactory()->newTCSProcessModule(
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

Observable<TCSObservableKinematic, TCSObservableResult>* TCSObservableService::newObservableModuleFromTask(
        const Task &task) const {

    //initialize
    Observable<TCSObservableKinematic, TCSObservableResult>* pObservable = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            TCSObservable::TCS_OBSERVABLE_MODULE_CLASS_NAME)) {

        //configure
        pObservable =
                Partons::getInstance()->getModuleObjectFactory()->newTCSObservable(
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
