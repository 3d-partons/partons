#include "ObservableResultList.h"

#include <sstream>

ObservableResultList::ObservableResultList() {
}

ObservableResultList::~ObservableResultList() {
}

void ObservableResultList::add(const ObservableResult& observableResult) {
    m_observableResultList.push_back(observableResult);
}

std::string ObservableResultList::toString() {
    std::ostringstream os;

    for (unsigned int i = 0; i != m_observableResultList.size(); i++) {
        os << m_observableResultList[i].toString() << std::endl;
    }

    return os.str();
}

void ObservableResultList::add(
        const ObservableResultList& observableResultList) {
    for (size_t i = 0; i != observableResultList.size(); i++) {
        m_observableResultList.push_back(observableResultList[i]);
    }
}

size_t ObservableResultList::size() const {
    return m_observableResultList.size();
}

ObservableResult& ObservableResultList::operator [](size_t n) {
    return m_observableResultList[n];
}

const ObservableResult& ObservableResultList::operator [](size_t n) const {
    return m_observableResultList[n];
}
