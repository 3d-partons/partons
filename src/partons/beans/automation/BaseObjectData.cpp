#include "../../../../include/partons/beans/automation/BaseObjectData.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <utility>

namespace PARTONS {


BaseObjectData::BaseObjectData() :
        BaseObject("BaseObjectData"), m_moduleType(
                ElemUtils::StringUtils::EMPTY), m_moduleClassName(
                ElemUtils::StringUtils::EMPTY) {
}

BaseObjectData::BaseObjectData(const std::string &moduleType,
        const std::string &moduleClassName) :
        BaseObject("BaseObjectData"), m_moduleType(moduleType), m_moduleClassName(
                moduleClassName) {
}

BaseObjectData::~BaseObjectData() {
}

BaseObjectData& BaseObjectData::addSubModule(const std::string& moduleType,
        const std::string& moduleClassName) {

    std::pair<std::map<std::string, BaseObjectData>::iterator, bool> isInserted;

    isInserted = m_subModules.insert(
            std::make_pair(moduleType,
                    BaseObjectData(moduleType, moduleClassName)));

    if (!isInserted.second) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Failed to insert " << moduleType
                        << "[" << moduleClassName << "]" << '\n' << toString());
    }

    return isInserted.first->second;
}

void BaseObjectData::addSubModule(const BaseObjectData& baseObjectData) {

    std::pair<std::map<std::string, BaseObjectData>::iterator, bool> isInserted;

    isInserted = m_subModules.insert(
            std::make_pair(baseObjectData.getModuleType(), baseObjectData));

    if (!isInserted.second) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Failed to insert " << baseObjectData.getModuleType()
                        << "[" << baseObjectData.getClassName() << "]" << '\n' << toString());
    }
}

void BaseObjectData::addParameter(const ElemUtils::Parameter& parameter) {
    m_parameters.add(parameter);
}

void BaseObjectData::addParameters(const ElemUtils::Parameters& parameters) {
    m_parameters.add(parameters);
}

std::string BaseObjectData::toString() const {
    ElemUtils::Formatter formatter;

    formatter << m_moduleType << "[" << m_moduleClassName
            << "] number of sub-module : " << m_subModules.size() << '\n'
            << m_parameters.toString() << '\n';

    for (std::map<std::string, BaseObjectData>::const_iterator it =
            m_subModules.begin(); it != m_subModules.end(); it++) {
        formatter << (it->second).toString();
    }

    return formatter.str();
}

bool BaseObjectData::isAvailableSubModule(
        const std::string& moduleClassType) const {
    m_it = m_subModules.find(moduleClassType);
    return (m_it != m_subModules.end()) ? true : false;
}

const BaseObjectData& BaseObjectData::getLastAvailable() const {
    return m_it->second;
}

const std::string& BaseObjectData::getModuleClassName() const {
    return m_moduleClassName;
}

void BaseObjectData::setModuleClassName(const std::string& moduleClassName) {
    m_moduleClassName = moduleClassName;
}

const std::string& BaseObjectData::getModuleType() const {
    return m_moduleType;
}

void BaseObjectData::setModuleType(const std::string& moduleType) {
    m_moduleType = moduleType;
}

const ElemUtils::Parameters& BaseObjectData::getParameters() const {
    return m_parameters;
}

void BaseObjectData::setParameters(const ElemUtils::Parameters& parameters) {
    m_parameters = parameters;
}

const std::map<std::string, BaseObjectData>& BaseObjectData::getSubModules() const {
    return m_subModules;
}

} /* namespace PARTONS */
