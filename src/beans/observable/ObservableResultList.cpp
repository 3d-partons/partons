#include "ObservableResultList.h"

#include <sstream>

ObservableResultList::ObservableResultList() :
        m_dateTime(time(0)) {
}

ObservableResultList::~ObservableResultList() {
}

// Copy object before add it to m_observableResultList ; we need to do it to set proper DateTime
void ObservableResultList::add(ObservableResult observableResult) {
    observableResult.setDateTime(getDateTime());
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
        add(observableResultList[i]);
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

time_t ObservableResultList::getDateTime() const {
    return m_dateTime;
}

void ObservableResultList::setDateTime(time_t dateTime) {
    m_dateTime = dateTime;
}
