#include "../../../../include/partons/services/automation/Qt4XMLValidator.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include <QtXmlPatterns/qxmlschema.h>
#include <QtXmlPatterns/qxmlschemavalidator.h>

#include "../../../../include/partons/services/automation/Qt4MessageHandler.h"

Qt4XMLValidator::Qt4XMLValidator() :
        XMLValidatorI("Qt4XMLValidator") {
}

Qt4XMLValidator::~Qt4XMLValidator() {
}

bool Qt4XMLValidator::isValidXMLDocument(const std::string &xmlSchemaStream,
        const std::string &xmlDocumentStream) const {

    if (xmlSchemaStream.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "XML schema is empty.");
    }
    if (xmlDocumentStream.empty()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "XML document is empty.");
    }

    const QByteArray schemaData = QString(xmlSchemaStream.c_str()).toUtf8();
    const QByteArray xmldata = QString(xmlDocumentStream.c_str()).toUtf8();

    Qt4MessageHandler messageHandler;
    bool isValid = false;

    QXmlSchema qSchema;
    qSchema.setMessageHandler(&messageHandler);
    qSchema.load(schemaData);

    if (qSchema.isValid()) {
        QXmlSchemaValidator validator(qSchema);
        isValid = validator.validate(xmldata);

        if (!isValid) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "XML document is invalid : "
                            << messageHandler.statusMessage());
        }
    }

    return isValid;
}

void Qt4XMLValidator::myMessageOutput(QtMsgType type, const char* msg) {
    switch (type) {
    case QtDebugMsg:
        ElemUtils::LoggerManager::getInstance()->debug("Qt4XMLValidator",
                __func__, msg);
        break;
    case QtWarningMsg:
        ElemUtils::LoggerManager::getInstance()->warn("Qt4XMLValidator",
                __func__, msg);
        break;
    case QtCriticalMsg:
        ElemUtils::LoggerManager::getInstance()->error("Qt4XMLValidator",
                __func__, msg);
        break;
    case QtFatalMsg:
        ElemUtils::LoggerManager::getInstance()->error("Qt4XMLValidator",
                __func__, msg);
        break;
    }
}
