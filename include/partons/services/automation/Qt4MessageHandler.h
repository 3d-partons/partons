#ifndef QT4MESSAGEHANDLER_H_
#define QT4MESSAGEHANDLER_H_

/**
 * @file Qt4MessageHandler.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 26, 2016
 * @version 1.0
 */

#include <Qt/qsourcelocation.h>
#include <QtCore/qglobal.h>
#include <QtCore/qstring.h>
#include <QtCore/qurl.h>
#include <QtXmlPatterns/qxmlschema.h>
#include <string>
#include <QAbstractMessageHandler>

/**
 * @class Qt4MessageHandler
 *
 * @brief
 */
class Qt4MessageHandler: public QAbstractMessageHandler {
public:
    Qt4MessageHandler();
    virtual ~Qt4MessageHandler();

    std::string statusMessage() const;

protected:
    virtual void handleMessage(QtMsgType type, const QString &description,
            const QUrl &identifier, const QSourceLocation &sourceLocation);

private:
    QtMsgType m_messageType;
    QString m_description;
    QSourceLocation m_sourceLocation;
};

#endif /* QT4MESSAGEHANDLER_H_ */
