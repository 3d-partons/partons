/*
 * ParameterList.h
 *
 *  Created on: Jul 3, 2015
 *      Author: Bryan BERTHOU
 */

#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

#include <stddef.h>
#include <map>
#include <string>

#include "../BaseObject.h"
#include "GenericType.h"

/*
 *
 */
class ParameterList: public BaseObject {
public:
    ParameterList();
    virtual ~ParameterList();

    GenericType get(const std::string &parameterName) const;

    size_t size();

private:
    // std::string : parameter name
    // GenericType : value of the parameter
    std::map<std::string, GenericType> m_parameters;
};

#endif /* PARAMETER_LIST_H */
