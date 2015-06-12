#ifndef SERVICE_FUNCTION_NAMES_H
#define SERVICE_FUNCTION_NAMES_H

/**
 * @file ServiceFunctionName
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 08 July 2015
 * @version 1.0
 *
 * @class ServiceFunctionNames
 *
 * @brief
 */

#include <string>

class ServiceFunctionNames {

public:

    enum Type {
        UNDEFINED = 0,

        GPD_SERVICE_COMPUTE_GPD_MODEL = 1,
        GPD_SERVICE_COMPUTE_GPD_MODEL_RESTRICTED_BY_GPD_TYPE = 2,
        GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION = 3,

        CFF_SERVICE_COMPUTE_WITH_GPD_MODEL,
        CFF_SERVICE_COMPUTE_WITH_CFF_MODEL,

        OBSERVABLE_SERVICE_COMPUTE_DVCS_OBSERVABLE_WITH_PHI_DEPENDENCY
    };

    ServiceFunctionNames();

    ServiceFunctionNames(Type type);

    ServiceFunctionNames(const std::string &string);

    /**
     * Copy constructor
     * @param other
     */
    ServiceFunctionNames(const ServiceFunctionNames &other);

    operator Type() const;

    std::string toString();

    ServiceFunctionNames::Type getType() const;

    void setType(Type type);

    ServiceFunctionNames::Type fromString(const std::string &string);

private:
    ServiceFunctionNames::Type m_type;
};

#endif /* SERVICE_FUNCTION_NAMES_H */
