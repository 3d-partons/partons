#include "../../../../include/partons/services/automation/AutomationService.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>
#include <ctime>

#include "../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../include/partons/beans/automation/Task.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/ResourceManager.h"
#include "../../../../include/partons/services/automation/DefaultXMLParser.h"
#include "../../../../include/partons/services/automation/Qt4XMLValidator.h"
#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

const std::string AutomationService::PROPERTY_NAME_XML_SCHEMA_FILE_PATH =
        "xml.schema.file.path";

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int AutomationService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new AutomationService("AutomationService"));

AutomationService::AutomationService(const std::string &className) :
        BaseObject(className), m_xmlSchemaFile(ElemUtils::StringUtils::EMPTY), m_pXMLValidatorI(
                0), m_pXMLParserI(0), m_pCryptographicHashService(0), m_pResourceManager(
                0) {
    // Plug Qt4 XML validator as XML validator.
    m_pXMLValidatorI = new Qt4XMLValidator();
    // Plug our own XML parser as XML parser
    m_pXMLParserI = new DefaultXMLParser();
}

AutomationService::~AutomationService() {
    // Free XML validator object memory
    if (m_pXMLValidatorI) {
        delete m_pXMLValidatorI;
        m_pXMLValidatorI = 0;
    }

    // Free XML parser object memory
    if (m_pXMLParserI) {
        delete m_pXMLParserI;
        m_pXMLParserI = 0;
    }
}

void AutomationService::resolveObjectDependencies() {
    m_xmlSchemaFile = ElemUtils::FileUtils::read(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    AutomationService::PROPERTY_NAME_XML_SCHEMA_FILE_PATH));

    m_pCryptographicHashService =
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService();

    m_pResourceManager = ResourceManager::getInstance();
}

const std::string& AutomationService::getXmlSchemaFile() const {
    return m_xmlSchemaFile;
}

Scenario* AutomationService::parseXMLFile(
        const std::string& xmlFilePath) const {

    std::string scenarioXMLDocument = ElemUtils::FileUtils::read(xmlFilePath);
    std::string scenarioHashSum =
            m_pCryptographicHashService->generateSHA1HashSum(
                    scenarioXMLDocument);

    Scenario* pScenario = m_pResourceManager->getScenarioByHashSum(
            scenarioHashSum);

    if (!pScenario) {
        pScenario = m_pResourceManager->registerScenario(-1,
                ElemUtils::StringUtils::EMPTY, time(0), xmlFilePath,
                scenarioHashSum, scenarioXMLDocument);
    }

    parseScenario(pScenario);

    return pScenario;
}

Scenario* AutomationService::parseScenario(Scenario* pScenario) const {
    if (!pScenario) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Scenario provided pointer is NULL");
    }

    if (!(m_pXMLValidatorI->isValidXMLDocument(getXmlSchemaFile(),
            pScenario->getFile()))) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "invalid scenario XML");
    }

    // if task list is empty, then parse XML file to fill task list.
    if (pScenario->size() == 0) {
        return m_pXMLParserI->parseScenario(pScenario);
    }

    return pScenario;
}

void AutomationService::playScenario(Scenario* pScenario) const {
    if (pScenario->size() == 0) {
        warn(__func__, "There is no task to perform in this scenario");
    } else {
        // compute each tasks found
        for (size_t i = 0; i < pScenario->size(); i++) {
            Task task = pScenario->getTask(i);
            Partons::getInstance()->getServiceObjectRegistry()->get(
                    task.getServiceName())->computeTask(task);
        }
    }
}
