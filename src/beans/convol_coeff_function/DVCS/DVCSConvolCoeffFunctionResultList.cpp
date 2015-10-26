#include "DVCSConvolCoeffFunctionResultList.h"

#include "../../../utils/stringUtils/Formatter.h"

DVCSConvolCoeffFunctionResultList::DVCSConvolCoeffFunctionResultList() {
}

DVCSConvolCoeffFunctionResultList::~DVCSConvolCoeffFunctionResultList() {
}

void DVCSConvolCoeffFunctionResultList::add(
        const DVCSConvolCoeffFunctionResult &result) {
    m_results.push_back(result);
}

const DVCSConvolCoeffFunctionResult& DVCSConvolCoeffFunctionResultList::get(
        unsigned int index) const {
    return m_results[index];
}

size_t DVCSConvolCoeffFunctionResultList::getSize() const {
    return m_results.size();
}

const DVCSConvolCoeffFunctionResult& DVCSConvolCoeffFunctionResultList::getUniqueResult() {
    return m_results[0];
}

std::string DVCSConvolCoeffFunctionResultList::toString() {
    Formatter formatter;

    for (unsigned int i = 0; i != m_results.size(); i++) {
        formatter << m_results[i].toString() << ' \n';
    }

    return formatter.str();
}
