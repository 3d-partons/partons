#ifndef DEFAULT_XML_VALIDATOR_H
#define DEFAULT_XML_VALIDATOR_H

/**
 * @file DefaultXMLValidator.h
 * @author Pawel Sznajder (NCBJ)
 * @date March 23, 2025
 * @version 1.0
 */

#include <string>

#include "XMLValidatorI.h"

namespace PARTONS {

/**
 * @class DefaultXMLValidator
 *
 * @brief Validator using libxml2 library.
 */
class DefaultXMLValidator: public XMLValidatorI {

public:

    DefaultXMLValidator();
    virtual ~DefaultXMLValidator();

    virtual bool isValidXMLDocument(const std::string &xmlSchemaStream,
            const std::string &xmlDocumentStream) const;
};

} /* namespace PARTONS */

#endif /* DEFAULT_XML_VALIDATOR_H */
