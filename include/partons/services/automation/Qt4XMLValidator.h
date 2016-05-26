#ifndef QT4_XML_VALIDATOR_H
#define QT4_XML_VALIDATOR_H

/**
 * @file Qt4XMLValidator.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 23, 2016
 * @version 1.0
 *
 * @class Qt4XMLValidator
 *
 * @brief
 */

#include <QtCore/qglobal.h>
#include <string>

#include "XMLValidatorI.h"

class Qt4XMLValidator: public XMLValidatorI {
public:
    Qt4XMLValidator();
    virtual ~Qt4XMLValidator();

    virtual bool isValidXMLDocument(const std::string &xmlSchemaStream,
            const std::string &xmlDocumentStream) const;

    static void myMessageOutput(QtMsgType type, const char *msg);
};

#endif /* QT4_XML_VALIDATOR_H */
