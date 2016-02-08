#include "../../../include/partons/utils/ParameterList.h"

#include "../../../include/partons/utils/stringUtils/Formatter.h"

ParameterList::ParameterList() :
        BaseObject("ParameterList") {
}

ParameterList::~ParameterList() {
}

GenericType ParameterList::get(const std::string& parameterName) const {
    std::map<std::string, GenericType>::const_iterator it = m_parameters.find(
            parameterName);

    if (it == m_parameters.end()) {
        error(__func__, "Missing parameter name = " + parameterName);
    }

    return it->second;
}

size_t ParameterList::size() {
    return m_parameters.size();
}

bool ParameterList::isAvailable(const std::string& parameterName) {
    m_it = m_parameters.find(parameterName);

    return (m_it != m_parameters.end()) ? true : false;
}

GenericType ParameterList::getLastAvailable() const {
    return m_it->second;
}

std::string ParameterList::toString() const {
    Formatter formatter;

    formatter << "[ParameterList::toString()]" << '\n';

    for (std::map<std::string, GenericType>::const_iterator it =
            m_parameters.begin(); it != m_parameters.end(); it++) {
        formatter << (it->first) << " = " << (it->second).toString() << '\n';
    }

    return formatter.str();
}

void ParameterList::clear() {
    m_parameters.clear();
}
