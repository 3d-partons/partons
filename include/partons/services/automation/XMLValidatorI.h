#ifndef XML_VALIDATOR_I_H
#define XML_VALIDATOR_I_H

/**
 * @file XMLValidatorI.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 23, 2016
 * @version 1.0
 */

#include <string>

#include "../../BaseObject.h"

/**
 * @class XMLValidatorI
 *
 * @brief
 */
class XMLValidatorI: public BaseObject {
public:
    XMLValidatorI(const std::string &className);
    virtual ~XMLValidatorI();

    virtual bool isValidXMLDocument(const std::string &xmlSchemaStream,
            const std::string &xmlDocumentStream) const = 0;
};

#endif /* XML_VALIDATOR_I_H */
