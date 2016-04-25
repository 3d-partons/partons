#include "../../../../include/partons/services/automation/XMLValidatorI.h"

#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

const std::string XMLValidatorI::PROPERTY_NAME_XML_SCHEMA_FILE_PATH =
        "xml.schema.file.path";

XMLValidatorI::XMLValidatorI(const std::string &className) :
        BaseObject(className), m_xmlSchemaFilePath(
                ElemUtils::StringUtils::EMPTY) {
}

XMLValidatorI::~XMLValidatorI() {
}

void XMLValidatorI::init() {
    m_xmlSchemaFilePath =
            ElemUtils::PropertiesManager::getInstance()->getString(
                    XMLValidatorI::PROPERTY_NAME_XML_SCHEMA_FILE_PATH);
}

const std::string& XMLValidatorI::getXmlSchemaFilePath() const {
    return m_xmlSchemaFilePath;
}
