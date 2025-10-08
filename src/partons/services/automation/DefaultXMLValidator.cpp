#include "../../../../include/partons/services/automation/DefaultXMLValidator.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>

#include <cstring>

namespace PARTONS {

DefaultXMLValidator::DefaultXMLValidator() :
        XMLValidatorI("DefaultXMLValidator") {
}

DefaultXMLValidator::~DefaultXMLValidator() {
}

bool DefaultXMLValidator::isValidXMLDocument(const std::string& xmlSchemaStream,
        const std::string& xmlDocumentStream) const {

        // 0) Init
        xmlInitParser();

        // 1) Parse schema from memory
        xmlSchemaParserCtxtPtr schemaParserCtxt =
                xmlSchemaNewMemParserCtxt(xmlSchemaStream.c_str(), xmlSchemaStream.size());
        xmlSchemaPtr schema = xmlSchemaParse(schemaParserCtxt);
        xmlSchemaFreeParserCtxt(schemaParserCtxt);

        if (! schema) {

                xmlCleanupParser();

                ElemUtils::LoggerManager::getInstance()->error("DefaultXMLValidator",
                        __func__, "Schema parsing failed");

                return false;
        }

        // 2) Parse XML document from memory
        xmlDocPtr doc = xmlReadMemory(xmlDocumentStream.c_str(), xmlDocumentStream.size(), "partons.xml", nullptr, 0);

        if (!doc) {

                xmlSchemaFree(schema);
                xmlCleanupParser();

                ElemUtils::LoggerManager::getInstance()->error("DefaultXMLValidator",
                        __func__, "XML parsing failed");

                return false;
        }

        // 3) Validate
        xmlSchemaValidCtxtPtr validCtxt = xmlSchemaNewValidCtxt(schema);
        int ret = xmlSchemaValidateDoc(validCtxt, doc);

        if (ret == 0) {
                ElemUtils::LoggerManager::getInstance()->info("DefaultXMLValidator",
                        __func__, "File valid");
        } else if (ret > 0) {
                ElemUtils::LoggerManager::getInstance()->error("DefaultXMLValidator",
                        __func__, "File invalid");
        } else {
                ElemUtils::LoggerManager::getInstance()->error("DefaultXMLValidator",
                        __func__, "Invalid validation");
        }

        // cleanup
        xmlSchemaFreeValidCtxt(validCtxt);
        xmlFreeDoc(doc);
        xmlSchemaFree(schema);
        xmlCleanupParser();

        // return
        return ret == 0;
}

} /* namespace PARTONS */
