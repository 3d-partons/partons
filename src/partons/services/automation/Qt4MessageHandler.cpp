#include "../../../../include/partons/services/automation/Qt4MessageHandler.h"

Qt4MessageHandler::Qt4MessageHandler() {
}

Qt4MessageHandler::~Qt4MessageHandler() {
}

std::string Qt4MessageHandler::statusMessage() const {
    return m_description.toStdString();
}

void Qt4MessageHandler::handleMessage(QtMsgType type,
        const QString& description, const QUrl& identifier,
        const QSourceLocation& sourceLocation) {
    Q_UNUSED(type);
    Q_UNUSED(identifier);

    m_messageType = type;
    m_description = description;
    m_sourceLocation = sourceLocation;
}
