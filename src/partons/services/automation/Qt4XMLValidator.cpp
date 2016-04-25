#include "../../../../include/partons/services/automation/Qt4XMLValidator.h"

#include <ElementaryUtils/logger/LoggerManager.h>
#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qstring.h>
#include <QtCore/qurl.h>
#include <QtXmlPatterns/qxmlschema.h>
#include <QtXmlPatterns/qxmlschemavalidator.h>

Qt4XMLValidator::Qt4XMLValidator() :
        XMLValidatorI("Qt4XMLValidator") {
}

Qt4XMLValidator::~Qt4XMLValidator() {
}

//TODO return info about errors if document invalid
bool Qt4XMLValidator::isValidXMLFile(const std::string& xmlFilePath) const {

    std::string xmlSchemaFilePath = getXmlSchemaFilePath();

    if (xmlSchemaFilePath.empty()) {
        error(__func__, "XML schema file path is empty !");
    }

    bool isValid = false;

    QUrl schemaUrl = QUrl::fromLocalFile(QString(xmlSchemaFilePath.c_str()));

    QXmlSchema qSchema;
    qSchema.load(schemaUrl);

    if (qSchema.isValid()) {
        QFile file(xmlFilePath.c_str());
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(qSchema);
        isValid = validator.validate(&file,
                QUrl::fromLocalFile(file.fileName()));

        if (isValid) {
            info(__func__, "instance document is valid");
            qDebug() << "instance document is valid";
        } else {
            qDebug() << "instance document is invalid";
            error(__func__, "instance document is invalid");

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
