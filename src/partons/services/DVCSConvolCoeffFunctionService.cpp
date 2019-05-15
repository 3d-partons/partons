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
#include "../../../include/partons/database/convol_coeff_function/service/DVCSConvolCoeffFunctionResultDaoService.h"
#include "../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
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

DVCSConvolCoeffFunctionKinematic DVCSConvolCoeffFunctionService::newKinematicFromTask(
        const Task& task) const {
    DVCSConvolCoeffFunctionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVCSConvolCoeffFunctionKinematic::DVCS_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<DVCSConvolCoeffFunctionKinematic> DVCSConvolCoeffFunctionService::newListOfKinematicFromTask(
        const Task& task) const {
    List<DVCSConvolCoeffFunctionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            DVCSConvolCoeffFunctionKinematic::DVCS_CONVOL_COEFF_FUNCTION_KNEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getDVCSCCFKinematicFromFile(
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

void DVCSConvolCoeffFunctionService::storeResultListInDatabase(
        const List<DVCSConvolCoeffFunctionResult>& results) const {

    //get dao service
    DVCSConvolCoeffFunctionResultDaoService dvcsConvolCoeffFunctionResultDaoService;

    //insert
    int computationId = dvcsConvolCoeffFunctionResultDaoService.insert(results);

    //check if inserted correctly
    if (computationId != -1) {
        info(__func__,
                ElemUtils::Formatter()
                        << "DVCSConvolCoeffFunctionResultList object has been stored in database with computation_id = "
                        << computationId);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "DVCSConvolCoeffFunctionResultList object : insertion into database failed");
    }
}

void DVCSConvolCoeffFunctionService::generatePlotFileTask(Task &task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task, "dvcs_ccf_plot_2d_view"),
            ' ');
}

ConvolCoeffFunctionModule<DVCSConvolCoeffFunctionKinematic,
        DVCSConvolCoeffFunctionResult>* DVCSConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
        const Task &task) const {

    //initialize
    ConvolCoeffFunctionModule<DVCSConvolCoeffFunctionKinematic,
            DVCSConvolCoeffFunctionResult>* pConvolCoeffFunctionModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            DVCSConvolCoeffFunctionModule::DVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME)) {

        //configure
        pConvolCoeffFunctionModule =
                Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pConvolCoeffFunctionModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pConvolCoeffFunctionModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "You have not provided any ConvolCoeffFunctionModule");
    }

    //return
    return pConvolCoeffFunctionModule;
}

} /* namespace PARTONS */
