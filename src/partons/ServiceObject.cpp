#include "../../include/partons/ServiceObject.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/MultimapParameters.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>

#include "../../include/partons/Partons.h"
#include "../../include/partons/ServiceObjectRegistry.h"
#include "../../include/partons/beans/automation/Task.h"

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

    if (task.isAvailableParameters("select")) {
        selectParams = task.getLastAvailableParameters();
    } else {
        error(__func__,
                "The select-type parameter is missing in the xml file ");
    }
    if (task.isAvailableParameters("where")) {
        whereParams = task.getLastAvailableParameters();
    } else {
        error(__func__, "The where-type parameter is missing in the xml file");
    }

    return generateSQLQueryForPlotFile(tableName, selectParams, whereParams);
}

std::string ServiceObject::generateSQLQueryForPlotFile(
        const std::string& tableName, const ElemUtils::Parameters& selectParams,
        const ElemUtils::Parameters& whereParams) const {
    ElemUtils::Formatter formatter;

    formatter << "SELECT ";

    if (selectParams.size() == 2) {
        formatter << selectParams.get("xPlot").toString() << ", "
                << selectParams.get("yPlot").toString();
    } else {
        error(__func__,
                "Missing xPlot or yPlot parameter in select-type xml element");
    }

    formatter << " FROM " << tableName << " WHERE ";

    for (unsigned int i = 0; i != whereParams.size(); i++) {
        formatter << whereParams.key(i) << " = "
                << whereParams.value(i).toString();
        if (i + 1 < whereParams.size()) {
            formatter << " AND ";
        }
    }

    debug(__func__, formatter.str());

    return formatter.str();
}

std::string ServiceObject::getOutputFilePathForPlotFileTask(Task& task) const {
    std::string filePath = ElemUtils::StringUtils::EMPTY;

    if (task.isAvailableParameters("output")) {
        filePath = task.getLastAvailableParameters().get("filePath").toString();
    } else {
        error(__func__, "The output-type parameter is missing in the xml file");
    }

    return filePath;
}
