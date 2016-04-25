#ifndef XML_VALIDATOR_I_H
#define XML_VALIDATOR_I_H

/**
 * @file XMLValidatorI.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 23, 2016
 * @version 1.0
 *
 * @class XMLValidatorI
 *
 * @brief
 */

#include <string>

#include "../../BaseObject.h"

class XMLValidatorI: public BaseObject {
public:
    XMLValidatorI(const std::string &className);
    virtual ~XMLValidatorI();

    virtual void init();

    virtual bool isValidXMLFile(const std::string &xmlFilePath) const = 0;

    const std::string& getXmlSchemaFilePath() const;

private:
    static const std::string PROPERTY_NAME_XML_SCHEMA_FILE_PATH;

    std::string m_xmlSchemaFilePath;
};

#endif /* XML_VALIDATOR_I_H */
