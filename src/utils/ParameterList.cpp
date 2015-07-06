/*
 * ParameterList.cpp
 *
 *  Created on: Jul 3, 2015
 *      Author: debian
 */

#include "ParameterList.h"

#include <utility>

ParameterList::ParameterList() :
        BaseObject("ParameterList") {
}

ParameterList::~ParameterList() {
    // TODO Auto-generated destructor stub
}

GenericType ParameterList::get(const std::string& parameterName) const {
    std::map<std::string, GenericType>::const_iterator it = m_parameters.find(
            parameterName);

    if (it == m_parameters.end()) {
        throwException(__func__, "Missing parameter name = " + parameterName);
    }

    return it->second;
}

size_t ParameterList::size() {
    return m_parameters.size();
}
