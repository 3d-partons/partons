#include "DVCSConvolCoeffFunctionResultList.h"

#include "../../../utils/stringUtils/Formatter.h"

DVCSConvolCoeffFunctionResultList::DVCSConvolCoeffFunctionResultList() :
        Computation() {
}

DVCSConvolCoeffFunctionResultList::~DVCSConvolCoeffFunctionResultList() {
}

// Copy object before add it to m_results ; we need to do it to set proper DateTime
void DVCSConvolCoeffFunctionResultList::add(
        DVCSConvolCoeffFunctionResult result) {
    result.setDateTime(getDateTime());
    m_results.push_back(result);
}

const DVCSConvolCoeffFunctionResult& DVCSConvolCoeffFunctionResultList::get(
        unsigned int index) const {
    return m_results[index];
}

size_t DVCSConvolCoeffFunctionResultList::size() const {
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

DVCSConvolCoeffFunctionResult& DVCSConvolCoeffFunctionResultList::operator [](
        size_t n) {
    return m_results[n];
}

const DVCSConvolCoeffFunctionResult& DVCSConvolCoeffFunctionResultList::operator [](
        size_t n) const {
    return m_results[n];
}
