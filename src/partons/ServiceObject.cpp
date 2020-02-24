#include "../../include/partons/ServiceObject.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>
#include <iostream>

#include "../../include/partons/beans/automation/BaseObjectData.h"
#include "../../include/partons/beans/automation/Scenario.h"
#include "../../include/partons/beans/automation/Task.h"
#include "../../include/partons/database/ResultDaoService.h"
#include "../../include/partons/Partons.h"
#include "../../include/partons/services/automation/XMLParserI.h"
#include "../../include/partons/ServiceObjectRegistry.h"
#include "../../include/partons/utils/plot2D/Plot2DList.h"

namespace PARTONS {

ServiceObject::ServiceObject(const std::string &className) :
        BaseObject(className), m_pModuleObjectFactory(0), m_pAutomationService(
                0) {
}

ServiceObject::~ServiceObject() {
    // Nothing to destroy
    // m_pModuleObjectFactory pointer will be deleted by the Partons class
}

void ServiceObject::addTasks(const List<ElemUtils::Packet>& tasks) {
    for (size_t i = 0; i != tasks.size(); i++) {
        m_queueOfTask.push(tasks[i]);
    }
}

bool ServiceObject::isEmptyTaskQueue() {
    return m_queueOfTask.empty();
}

ElemUtils::Packet ServiceObject::popTaskFormQueue() {
    return m_queueOfTask.pop();
}

void ServiceObject::initComputationalThread(ModuleObject* pModuleObject) {
    unsigned int nbOfThread = ElemUtils::GenericType(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "computation.nb.processor")).toUInt();

    info(__func__,
            ElemUtils::Formatter() << "Creation of thread : " << nbOfThread);

    m_threadManager.newThread(nbOfThread, pModuleObject);
}

void ServiceObject::launchAllThreadAndWaitingFor() {
    m_threadManager.launchAllAndWaitingFor();
}

void ServiceObject::clearAllThread() {
    m_threadManager.clearAllThread();
}

void ServiceObject::resolveObjectDependencies() {
    BaseObject::resolveObjectDependencies();

    if (!m_pModuleObjectFactory) {
        m_pModuleObjectFactory =
                Partons::getInstance()->getModuleObjectFactory();
    }

    if (!m_pAutomationService) {
        m_pAutomationService =
                Partons::getInstance()->getServiceObjectRegistry()->getAutomationService();
    }
}

std::string ServiceObject::generateSQLQueryForPlotFileTask(Task &task,
        const std::string &tableName) const {
    ElemUtils::Parameters selectParams, whereParams;

//    if (task.isAvailableParameters("select")) {
//        selectParams = task.getLastAvailableParameters();
//    } else {
//        throw ElemUtils::CustomException(getClassName(), __func__,
//                "The select-type parameter is missing in the xml file ");
//    }
//    if (task.isAvailableParameters("where")) {
//        whereParams = task.getLastAvailableParameters();
//    } else {
//        throw ElemUtils::CustomException(getClassName(), __func__,
//                "The where-type parameter is missing in the xml file");
//    }

    if (task.getTaskParameters().isAvailableSubModule("select")) {
        selectParams =
                task.getTaskParameters().getLastAvailable().getParameters();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "The select-type parameter is missing in the xml file ");
    }
    if (task.getTaskParameters().isAvailableSubModule("where")) {
        whereParams =
                task.getTaskParameters().getLastAvailable().getParameters();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "The where-type parameter is missing in the xml file");
    }

    return generateSQLQueryForPlotFile(tableName, selectParams, whereParams);
}

std::string ServiceObject::generateSQLQueryForPlotFile(
        const std::string& tableName, const ElemUtils::Parameters& selectParams,
        const ElemUtils::Parameters& whereParams) const {
    ElemUtils::Formatter formatter;

    formatter << "SELECT ";

    if (selectParams.size() == 2) {
        formatter << selectParams.get("xPlot").getString() << ", " << selectParams.get("xPlot").getString() << "_unit" << ", "
                << selectParams.get("yPlot").getString() << ", " << selectParams.get("yPlot").getString() << "_unit";
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Missing xPlot or yPlot parameter in select-type xml element");
    }

    formatter << " FROM " << tableName << " WHERE ";

    for (unsigned int i = 0; i != whereParams.size(); i++) {
        formatter << whereParams.key(i) << " = '" << whereParams.stringValue(i)
                << "'";
        if (i + 1 < whereParams.size()) {
            formatter << " AND ";
        }
    }

    debug(__func__, formatter.str());

    return formatter.str();
}

std::string ServiceObject::getOutputFilePathForPlotFileTask(Task& task) const {
    std::string filePath = ElemUtils::StringUtils::EMPTY;

    if (task.getTaskParameters().isAvailableSubModule("output")) {
        filePath =
                task.getTaskParameters().getLastAvailable().getParameters().get(
                        "filePath").getString();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "The output-type parameter is missing in the xml file");
    }

    return filePath;
}

void ServiceObject::generatePlotFile(const std::string& filePath,
        const std::string& sqlQuery, const char splitChar) const {
    Plot2DList plot2DList = ResultDaoService::getPlot2DListFromCustomQuery(
            sqlQuery);

    if (plot2DList.isEmpty()) {
        warn(__func__,
                "There is no coincidence all over the database with the given parameters");
    } else {
        info(__func__,
                ElemUtils::Formatter() << plot2DList.size()
                        << " data in coincidence with the given parameters have been found in the database and written in the file : "
                        << filePath);

    }

    std::ofstream fileOutputStream;

    if (!ElemUtils::FileUtils::open(fileOutputStream, filePath)) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Cannot open \"" << filePath
                        << "\" to store plot values");
    }

    ElemUtils::FileUtils::writeAndFlush(fileOutputStream,
            plot2DList.toStringPlotFile(splitChar));

    ElemUtils::FileUtils::close(fileOutputStream);
}

List<GPDType> ServiceObject::getGPDTypeListFromTask(Task& task) const {

    debug(__func__, task.toString());

    List<GPDType> gpdTypeList;

    //TODO replace by static variable
    if (task.getTaskParameters().isAvailableSubModule("GPDType")) {
        try {
            //TODO replace string GPDType by static variable
            gpdTypeList =
                    GPDType::getListOfGPDTypeFromString(
                            task.getTaskParameters().getLastAvailable().getParameters().get(
                                    "value").getString());
        } catch (const std::exception &e) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << e.what() << " for <"
                            << XMLParserI::TASK_PARAM_NODE_NAME
                            << " type=\"GPDType\"> element. Please check your XML file.");
        }
    } else {
        debug(__func__, "No task_param XML element");
    }

    debug(__func__,
            ElemUtils::Formatter() << "GPDTypeList size = "
                    << gpdTypeList.size());

    return gpdTypeList;
}

void ServiceObject::errorUnknownMethod(const Task& task) const {
    throw ElemUtils::CustomException(getClassName(), __func__,
            ElemUtils::Formatter() << "Unknown method = \""
                    << task.getFunctionName()
                    << "\" ; Please check task number ["
                    << task.getScenarioTaskIndexNumber()
                    << "] in your scenario file = \""
                    << task.getScenario()->getFilePath() << "\"");
}

} /* namespace PARTONS */
