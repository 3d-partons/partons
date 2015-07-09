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

ParameterList::ParameterList(const std::string& parameterName,
        const std::string& parameterValue) :
        BaseObject("ParameterList") {
    add(parameterName, parameterValue);
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

void ParameterList::add(const std::string& parameterName,
        const std::string& parameterValue) {
    m_parameters.insert(
            std::make_pair(parameterName, GenericType(parameterValue)));
}

