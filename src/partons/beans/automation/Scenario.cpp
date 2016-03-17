#include "../../../../include/partons/beans/automation/Scenario.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

Scenario::Scenario() :
        m_id(ElemUtils::StringUtils::EMPTY), m_date(), m_description(
                ElemUtils::StringUtils::EMPTY) {
}

Scenario::~Scenario() {
}

//void Scenario::addFunctionArg(std::string argName, void* argValue) {
//    m_functionArgs.insert(std::make_pair(argName, argValue));
//}
//
//const std::map<std::string, void*>& Scenario::getFunctionArgs() const {
//    return m_functionArgs;
//}
//
//void Scenario::setFunctionArgs(
//        const std::map<std::string, void*>& functionArgs) {
//    this->m_functionArgs = functionArgs;
//}

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

size_t Scenario::size() const {
    return m_tasks.size();
}

//TODO test range of i and throw exception
const Task& Scenario::getTask(unsigned int i) const {
    return m_tasks[i];
}

Task& Scenario::getTask(unsigned int i) {
    return m_tasks[i];
}

void Scenario::add(const Task& task) {
    m_tasks.push_back(task);
}
