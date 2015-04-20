/*
 * Scenario.cpp
 *
 *  Created on: 30 janv. 2015
 *      Author: bryan
 */

#include "Scenario.h"

#include <utility>

Scenario::Scenario() {
    // TODO Auto-generated constructor stub

}

Scenario::~Scenario() {
    // TODO Auto-generated destructor stub
}

void Scenario::addFunctionArg(std::string argName, void* argValue) {
    m_functionArgs.insert(std::make_pair(argName, argValue));
}

const std::map<std::string, void*>& Scenario::getFunctionArgs() const {
    return m_functionArgs;
}

void Scenario::setFunctionArgs(
        const std::map<std::string, void*>& functionArgs) {
    this->m_functionArgs = functionArgs;
}

const std::string& Scenario::getFunctionName() const {
    return m_functionName;
}

void Scenario::setFunctionName(const std::string& functionName) {
    this->m_functionName = functionName;
}

time_t Scenario::getDate() const {
    return m_date;
}

void Scenario::setDate(time_t date) {
    m_date = date;
}

const std::string& Scenario::getDescription() const {
    return m_description;
}

void Scenario::setDescription(const std::string& description) {
    m_description = description;
}

const std::string& Scenario::getId() const {
    return m_id;
}

void Scenario::setId(const std::string& id) {
    m_id = id;
}

const std::string& Scenario::getServiceName() const {
    return m_serviceName;
}

void Scenario::setServiceName(const std::string& serviceName) {
    this->m_serviceName = serviceName;
}
